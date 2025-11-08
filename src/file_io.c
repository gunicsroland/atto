#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "config.h"
#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <stdarg.h>
#include <stdbool.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <time.h>

#include "../include/file_io.h"
#include "../include/data.h"
#include "../include/editor_i.h"
#include "../include/editor_o.h"
#include "../include/row_operations.h"
#include "../include/syntax_highlighting.h"
#include "../include/terminal.h"

char* editorRowsToString(struct editorConfig* editor, int* buflen)
{
    int totlen = 0;
    int j;

    for (j = 0; j < editor->numrows; j++)
        totlen += editor->row[j].size + 1;
    *buflen = totlen;

    char* buf = malloc(totlen);
    char* p = buf;
    for (j = 0; j < editor->numrows; j++)
    {
        memcpy(p, editor->row[j].chars, editor->row[j].size);
        p += editor->row[j].size;
        *p = '\n';
        p++;
    }

    return buf;
}

void editorOpen(struct editorConfig* editor, char* filename)
{
    printf("editor->filename before free = %p\n", (void*)editor->filename);

    free(editor->filename);
    editor->filename = strdup(filename);

    FILE* filep = fopen(filename, "r");
    if (!filep)
        filep = fopen(filename, "a");
    if (!filep)
        die("fopen");

    editorSelectSyntaxHighlight(editor);

    char* line = NULL;
    size_t linecap = 0;
    ssize_t linelen;
    while ((linelen = getline(&line, &linecap, filep)) != -1)
    {
        while (linelen > 0 && (line[linelen - 1] == '\n' || line[linelen - 1] == '\r'))
            linelen--;
        editorInsertRow(editor, editor->numrows, line, linelen);
    }
    free(line);
    fclose(filep);
    editor->num_width = editorGetNumLen(editor->numrows - 1);
    editor->dirty = 0;
}

void editorSave(struct editorConfig* editor)
{
    if (editor->filename == NULL)
    {
        editor->filename = editorPrompt(editor, "Save as: %s", NULL);
        if (editor->filename == NULL)
        {
            editorSetStatusMessage(editor, "Save aborted");
            return;
        }
        editorSelectSyntaxHighlight(editor);
    }

    int len;
    char* buf = editorRowsToString(editor, &len);

    int fd = open(editor->filename, O_RDWR | O_CREAT, 0644);
    if (fd != -1)
    {
        if (ftruncate(fd, len) != -1)
        {
            if (write(fd, buf, len) == len)
            {
                close(fd);
                free(buf);
                editor->dirty = 0;
                editorSetStatusMessage(editor,"%d bytes written to disk", len);
                return;
            }
        }
        close(fd);
    }

    free(buf);
    editorSetStatusMessage(editor, "Can't save! I/O error: %s", strerror(errno));
}