#include "../include/editor_operations.h"
#include "../include/data.h"
#include "../include/row_operations.h"

void editorInsertChar(struct editorConfig* editor, int c)
{
    if (editor->cy == editor->numrows)
    {
        editorInsertRow(editor, editor->numrows, "", 0);
    }
    editorRowInsertChar(editor, &editor->row[editor->cy],
                        editor->cx, c);
    editor->cx++;
}

void editorDelChar(struct editorConfig* editor)
{
    if (editor->cy == editor->numrows)
        return;
    if (editor->cx == 0 && editor->cy == 0)
        return;

    erow* row = &editor->row[editor->cy];
    if (editor->cx > 0)
    {
        editorRowDelChar(editor, row, editor->cx - 1);
        editor->cx--;
    }
    else
    {
        editor->cx = editor->row[editor->cy - 1].size;
        editorRowAppendString(editor,
                              &editor->row[editor->cy - 1],
                              row->chars, row->size);
        editorDelRow(editor, editor->cy);
        editor->cy--;
    }
}

void editorInsertNewLine(struct editorConfig* editor)
{
    if (editor->cx == 0)
    {
        editorInsertRow(editor, editor->cy, "", 0);
    }
    else
    {
        erow* row = &editor->row[editor->cy];
        editorInsertRow(editor, editor->cy + 1,
                        &row->chars[editor->cx],
                        row->size - editor->cx);
        row = &editor->row[editor->cy];
        row->size = editor->cx;
        row->chars[row->size] = '\0';
        editorUpdateRow(editor, row);
    }

    int tabs = 0;
    if (editor->cy >= 0)
    {
        tabs = 0;
        for (int j = 0; j < editor->row[editor->cy].size;
             j++)
            if (editor->row[editor->cy].chars[j] == '\t')
                tabs++;

        for (int j = 0; j < tabs; j++)
        {
            editorRowInsertChar(
                editor, &editor->row[editor->cy + 1], 0,
                '\t');
        }
    }

    editor->cy++;
    editor->cx = tabs;
}