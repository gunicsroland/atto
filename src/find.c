#include "../include/find.h"
#include "../include/config.h"
#include "../include/data.h"
#include "../include/editor_i.h"
#include "../include/row_operations.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void editorFindCallback(struct editorConfig* editor,
                        char* query, int key)
{
    static int last_match = -1;
    static int direction = 1;

    static int saved_hl_line;
    static char* saved_hl = NULL;

    if (saved_hl)
    {
        memcpy(editor->row[saved_hl_line].hl, saved_hl,
               editor->row[saved_hl_line].rsize);
        free(saved_hl);
        saved_hl = NULL;
    }

    if (key == '\r' || key == '\x1b')
    {
        last_match = -1;
        direction = 1;
        return;
    }
    else if (key == ARROW_RIGHT || key == ARROW_DOWN)
    {
        direction = 1;
    }
    else if (key == ARROW_LEFT || key == ARROW_UP)
    {
        direction = -1;
    }
    else
    {
        last_match = -1;
        direction = 1;
    }

    if (last_match == -1)
        direction = 1;
    int current = last_match;
    int i;
    for (i = 0; i < editor->numrows; i++)
    {
        current += direction;
        if (current == -1)
            current = editor->numrows - 1;
        else if (current == editor->numrows)
            current = 0;

        erow* row = &editor->row[current];
        char* match = strstr(row->render, query);
        if (match)
        {
            last_match = current;
            editor->cy = current;
            editor->cx =
                editorRowRxToCx(row, match - row->render);
            editor->rowoff = editor->numrows;

            saved_hl_line = current;
            saved_hl = malloc(row->rsize);
            memcpy(saved_hl, row->hl, row->rsize);
            memset(&row->hl[match - row->render], HL_MATCH,
                   strlen(query));
            break;
        }
    }
}

void editorFind(struct editorConfig* editor)
{
    int saved_cx = editor->cx;
    int saved_cy = editor->cy;
    int saved_coloff = editor->coloff;
    int saved_rowoff = editor->rowoff;

    printf("Initiating find\n");
    char* query = editorPrompt(
        editor, "Search: %s (Use Esc/Enter/Arrows)",
        editorFindCallback);
    if (query)
        free(query);
    else
    {
        editor->cx = saved_cx;
        editor->cy = saved_cy;
        editor->coloff = saved_coloff;
        editor->rowoff = saved_rowoff;
    }
}

void editorFindLine(struct editorConfig* editor)
{
    char* query = editorPrompt(
        editor, "Search Line Number: %s (Use Esc/Enter)",
        NULL);
    if (query == NULL || editor->numrows == 0)
        return;

    int num;
    if (sscanf(query, "%d", &num) != 1)
        return;
    if (num <= 0)
        return;
    if (num > editor->numrows)
        num = editor->numrows;

    editor->cy = num - 1;
    editor->cx = 0;
    editor->rowoff = editor->numrows;

    free(query);
}