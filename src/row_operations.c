#include "../include/row_operations.h"
#include "../include/config.h"
#include "../include/data.h"
#include "../include/syntax_highlighting.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int editorGetNumLen(int num)
{
    int l = 1;
    while (num > 9)
    {
        l++;
        num /= 10;
    }

    return l;
}

int editorRowCxToRx(erow* row, int cx)
{
    int rx = 0;
    int j = 0;
    for (j = 0; j < cx; j++)
    {
        if (row->chars[j] == '\t')
            rx += (ATTO_TAB_STOP - 1) - (rx % ATTO_TAB_STOP);
        rx++;
    }

    return rx;
}

int editorRowRxToCx(erow* row, int rx)
{
    int cur_rx = 0;
    int cx;
    for (cx = 0; cx < row->size; cx++)
    {
        if (row->chars[cx] == '\t')
            cur_rx += (ATTO_TAB_STOP - 1) - (cur_rx % ATTO_TAB_STOP);
        cur_rx++;

        if (cur_rx > rx)
            return cx;
    }
    return cx;
}

void editorUpdateRow(struct editorConfig* editor, erow* row)
{
    int tabs = 0;
    int j;
    for (j = 0; j < row->size; j++)
        if (row->chars[j] == '\t')
            tabs++;

    free(row->render);
    row->render = malloc(row->size + tabs * (ATTO_TAB_STOP - 1) + 1);

    int idx = 0;
    for (j = 0; j < row->size; j++)
    {
        if (row->chars[j] == '\t')
        {
            row->render[idx++] = ' ';
            while (idx % ATTO_TAB_STOP != 0)
                row->render[idx++] = ' ';
        }
        else
        {
            row->render[idx++] = row->chars[j];
        }
    }
    row->render[idx] = '\0';
    row->rsize = idx;

    editorUpdateSyntax(editor,row);
}

void editorInsertRow(struct editorConfig* editor, int at, char* s, size_t len)
{
    if (at < 0 || editor->numrows < at)
        return;

    editor->row = realloc(editor->row, sizeof(erow) * (editor->numrows + 1));
    memmove(&editor->row[at + 1], &editor->row[at], sizeof(erow) * (editor->numrows - at));
    for (int j = at + 1; j <= editor->numrows; j++)
        editor->row[j].idx++;

    editor->row[at].idx = at;

    editor->row[at].size = len;
    editor->row[at].chars = malloc(len + 1);
    memcpy(editor->row[at].chars, s, len);
    editor->row[at].chars[len] = '\0';

    editor->row[at].rsize = 0;
    editor->row[at].render = NULL;
    editor->row[at].hl = NULL;
    editor->row[at].hl_open_comment = 0;

    editorUpdateRow(editor, &editor->row[at]);

    editor->numrows++;
    editor->dirty++;

    editor->num_width = editorGetNumLen(editor->numrows);
}

void editorFreeRow(erow* row)
{
    free(row->render);
    free(row->chars);
    free(row->hl);
}

void editorDelRow(struct editorConfig* editor, int at)
{
    if (at < 0 || editor->numrows <= at)
        return;
    editorFreeRow(&editor->row[at]);
    memmove(&editor->row[at], &editor->row[at + 1], sizeof(erow) * (editor->numrows - at - 1));
    for (int j = at; j < editor->numrows - 1; j++)
        editor->row[j].idx--;
    editor->numrows--;
    editor->dirty++;

    editor->num_width = editorGetNumLen(editor->numrows);
}

void editorRowInsertChar(struct editorConfig* editor, erow* row, int at, int c)
{
    if (at < 0 || row->size < at)
        at = row->size;
    row->chars = realloc(row->chars, row->size + 2);
    memmove(&row->chars[at + 1], &row->chars[at], row->size - at + 1);
    row->size++;
    row->chars[at] = c;
    editorUpdateRow(editor, row);
    editor->dirty++;
}

void editorRowAppendString(struct editorConfig* editor, erow* row, char* s, size_t len)
{
    if (len == 0)
        return;

    row->chars = realloc(row->chars, row->size + len + 1);
    memcpy(&row->chars[row->size], s, len);
    row->size += len;
    row->chars[row->size] = '\0';
    editorUpdateRow(editor, row);
    editor->dirty++;
}

void editorRowDelChar(struct editorConfig* editor, erow* row, int at)
{
    if (at < 0 || row->size <= at)
        return;
    memmove(&row->chars[at], &row->chars[at + 1], row->size - at);
    row->size--;
    editorUpdateRow(editor, row);
    editor->dirty++;
}