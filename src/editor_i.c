#include "../include/editor_i.h"
#include "../include/config.h"
#include "../include/data.h"
#include "../include/editor_o.h"
#include "../include/editor_operations.h"
#include "../include/file_io.h"
#include "../include/find.h"
#include "../include/row_operations.h"
#include "../include/terminal.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

char* editorPrompt(struct editorConfig* editor, char* prompt, void (*callback)(struct editorConfig*,char*, int))
{
    size_t bufsize = 128;
    char* buf = malloc(bufsize);

    size_t buflen = 0;
    buf[0] = '\0';

    while (1)
    {
        editorSetStatusMessage(editor, prompt, buf);
        editorRefreshScreen(editor);

        int c = editorReadKey();
        if (c == DEL_KEY || c == CTRL_KEY('h') || c == BACKSPACE)
        {
            if (buflen != 0)
                buf[--buflen] = '\0';
        }
        else if (c == '\x1b')
        {
            editorSetStatusMessage(editor, "");
            if (callback)
                callback(editor, buf, c);
            free(buf);
            return NULL;
        }
        else if (c == '\r')
        {
            if (buflen != 0)
            {
                editorSetStatusMessage(editor, "");
                if (callback)
                    callback(editor, buf, c);
                return buf;
            }
        }
        else if (!iscntrl(c) && c < 128)
        {
            if (buflen == bufsize - 1)
            {
                bufsize *= 2;
                buf = realloc(buf, bufsize);
            }
            buf[buflen++] = c;
            buf[buflen] = '\0';
        }

        if (callback)
            callback(editor, buf, c);
    }
}

void editorMoveCursor(struct editorConfig* editor, int key)
{
    erow* row = (editor->cy < editor->numrows) ? &editor->row[editor->cy] : NULL;

    switch (key)
    {
    case ARROW_LEFT:
        if (editor->cx != 0)
            editor->cx--;
        else if (editor->cy > 0)
        {
            editor->cy--;
            editor->cx = editor->row[editor->cy].size;
        }
        break;
    case ARROW_RIGHT:
        if (row && editor->cx < row->size)
            editor->cx++;
        else if (row && editor->cx == row->size)
        {
            editor->cy++;
            editor->cx = 0;
        }
        break;
    case ARROW_UP:
        if (editor->cy != 0)
            editor->cy--;
        break;
    case ARROW_DOWN:
        if (editor->cy < editor->numrows)
            editor->cy++;
        break;
    }

    row = (editor->cy < editor->numrows) ? &editor->row[editor->cy] : NULL;
    int rowlen = row ? row->size : 0;
    if (editor->cx > rowlen)
        editor->cx = rowlen;
}

void editorProcessKeypress(struct editorConfig* editor)
{
    static int quit_times = ATTO_QUIT_TIMES;
    int c = editorReadKey();
    switch (c)
    {

    case '\r':
        editorInsertNewLine(editor);
        break;

    case CTRL_KEY('q'):
        if (editor->dirty && quit_times > 0)
        {
            editorSetStatusMessage(editor, "Warning! File ha unsaved changes. "
                                   "Press ^Q %d more times to quit.",
                                   quit_times);
            quit_times--;
            return;
        }

        if (editor->row)
            editorFreeRow(editor->row);

        if (editor->filename)
        {
            free(editor->filename);
            editor->filename = NULL;
        }

        write(STDOUT_FILENO, "\x1b[2J", 4);
        write(STDOUT_FILENO, "\x1b[H", 3);
        exit(0);
        break;

    case CTRL_KEY('s'):
        editorSave(editor);
        break;

    case HOME_KEY:
        editor->cx = 0;
        break;
    case END_KEY:
        if (editor->cy < editor->numrows)
            editor->cx = editor->row[editor->cy].size;
        break;
    case CTRL_KEY('f'):
        editorFind(editor);
        break;

    case BACKSPACE:
    case CTRL_KEY('h'):
    case DEL_KEY:
        if (c == DEL_KEY)
            editorMoveCursor(editor, ARROW_RIGHT);
        editorDelChar(editor);
        break;

    case PAGE_UP:
    case PAGE_DOWN:
    {
        if (c == PAGE_UP)
        {
            editor->cy = editor->rowoff;
        }
        else if (c == PAGE_DOWN)
        {
            editor->cy = editor->rowoff + editor->screenrows - 1;
            if (editor->cy > editor->numrows)
                editor->cy = editor->numrows;
        }

        int times = editor->screenrows;
        while (times--)
            editorMoveCursor(editor, c == PAGE_UP ? ARROW_UP : ARROW_DOWN);
    }
    break;

    case ARROW_UP:
    case ARROW_DOWN:
    case ARROW_LEFT:
    case ARROW_RIGHT:
        editorMoveCursor(editor, c);
        break;

    case CTRL_KEY('l'):
        editorFindLine(editor);
    case '\x1b':
        break;
        
    case CTRL_KEY('j'):
        if(current_editor > 0){
            current_editor--;
        }
        editorSetStatusMessage(&Editors[current_editor], "Switched to editor %d", current_editor + 1);
        break;

    case CTRL_KEY('k'):
        if(current_editor < editor_num - 1){
            current_editor++;
        }
        editorSetStatusMessage(&Editors[current_editor], "Switched to editor %d", current_editor + 1);
        break;
        
    default:
        editorInsertChar(editor, c);
        break;
    }

    quit_times = ATTO_QUIT_TIMES;
}