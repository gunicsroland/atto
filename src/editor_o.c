#include "../include/editor_o.h"
#include "../include/append_buffer.h"
#include "../include/config.h"
#include "../include/data.h"
#include "../include/row_operations.h"
#include "../include/syntax_highlighting.h"
#include "../include/terminal.h"

#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void editorScroll(struct editorConfig* editor)
{
    editor->rx = 0;
    if (editor->cy < editor->numrows)
    {
        editor->rx = editorRowCxToRx(
            &editor->row[editor->cy], editor->cx);
    }

    if (editor->cy < editor->rowoff)
    {
        editor->rowoff = editor->cy;
    }
    if (editor->cy >= editor->rowoff + editor->screenrows)
    {
        editor->rowoff =
            editor->cy - editor->screenrows + 1;
    }
    if (editor->rx < editor->coloff)
    {
        editor->coloff = editor->rx;
    }
    if (editor->rx >= editor->coloff + editor->screencols)
    {
        editor->coloff =
            editor->rx - editor->screencols + 1;
    }
}

void editorDrawRows(struct editorConfig* editor,
                    struct abuf* ab)
{
    int y;
    int filerow;
    int padding;
    for (y = 0; y < editor->screenrows; y++)
    {
        filerow = y + editor->rowoff;
        char buf[32];
        snprintf(buf, sizeof(buf), "\x1b[%d;%dH", y + 1,
                 editor->start_x + 1);
        abAppend(ab, buf, strlen(buf));

        if (filerow >= editor->numrows)
        {
            if (editor->numrows == 0 &&
                y == editor->screenrows / 3)
            {
                char welcome[80];
                int welcomelen =
                    snprintf(welcome, sizeof(welcome),
                             "Atto editor -- version %s",
                             ATTO_VERSION);
                if (welcomelen > editor->width)
                    welcomelen = editor->width;

                padding = (editor->width - welcomelen) / 2;
                if (padding)
                {
                    abAppend(ab, "~", 1);
                    padding--;
                }
                while (padding--)
                    abAppend(ab, " ", 1);
                abAppend(ab, welcome, welcomelen);
            }
            else
            {
                abAppend(ab, "~", 1);
            }
        }
        else
        {
            int len =
                editor->row[filerow].rsize - editor->coloff;
            if (len < 0)
                len = 0;
            if (len > editor->width)
                len = editor->width;
            char* c = &editor->row[filerow]
                           .render[editor->coloff];
            unsigned char* hl =
                &editor->row[filerow].hl[editor->coloff];
            int current_color = -1;
            int j;

            char line_num_buf[50];
            int line_num_len = snprintf(
                line_num_buf, sizeof(line_num_buf), "%*d ",
                editor->num_width, filerow + 1);
            abAppend(ab, "\x1b[90m", 5);
            abAppend(ab, line_num_buf, line_num_len);
            abAppend(ab, "\x1b[39m", 5);

            for (j = 0; j < len; j++)
            {
                if (iscntrl(c[j]))
                {
                    char sym =
                        (c[j] <= 26) ? '@' + c[j] : '?';
                    abAppend(ab, "\x1b[7m", 4);
                    abAppend(ab, &sym, 1);
                    abAppend(ab, "\x1b[m", 3);
                    if (current_color != -1)
                    {
                        char buf[16];
                        int clen = snprintf(
                            buf, sizeof(buf),
                            "\x1b[38;5;%dm", current_color);
                        abAppend(ab, buf, clen);
                    }
                }
                else if (hl[j] == HL_NORMAL)
                {
                    if (current_color != -1)
                    {
                        abAppend(ab, "\x1b[39;49m", 8);
                        current_color = -1;
                    }
                    abAppend(ab, &c[j], 1);
                }
                else
                {
                    int color = hl[j];
                    if (color != current_color)
                    {
                        current_color = color;
                        char buf[16];
                        int clen = snprintf(
                            buf, sizeof(buf),
                            "\x1b[38;5;%dm", color);
                        abAppend(ab, buf, clen);
                    }
                    abAppend(ab, &c[j], 1);
                }
            }
            abAppend(ab, "\x1b[39m", 5);
        }

        abAppend(ab, "\x1b[K", 3);
        abAppend(ab, "\r\n", 2);
    }
}

void editorDrawStatusBar(struct editorConfig* editor,
                         struct abuf* ab)
{
    abAppend(ab, "\x1b[7m", 4);
    char status[80], rstatus[80];

    int len = snprintf(
        status, sizeof(status), "%.20s - %d lines %s",
        editor->filename ? editor->filename : "[No Name]",
        editor->numrows, editor->dirty ? "(modified)" : "");
    int rlen = snprintf(
        rstatus, sizeof(rstatus), "%s | %d/%d | %d/%d",
        editor->syntax ? editor->syntax->filetype : "no ft",
        editor->cy + 1, editor->numrows, editor->cx,
        editor->row ? editor->row[editor->cy].size : 0);

    if (len > editor->screencols)
        len = editor->screencols;
    abAppend(ab, status, len);

    while (len < editor->screencols)
    {
        if (editor->screencols - len == rlen)
        {
            abAppend(ab, rstatus, rlen);
            break;
        }
        abAppend(ab, " ", 1);
        len++;
    }
    abAppend(ab, "\x1b[m", 3);
    abAppend(ab, "\r\n", 2);
}

void editorDrawMessageBar(struct editorConfig* editor,
                          struct abuf* ab)
{
    abAppend(ab, "\x1b[K", 3);
    int msglen = strlen(editor->statusmsg);
    if (msglen > editor->screencols)
        msglen = editor->screencols;
    if (msglen)
        abAppend(ab, editor->statusmsg, msglen);
}

void editorRefreshScreen(struct editorConfig* editor)
{
    editorScroll(editor);

    struct abuf ab = ABUF_INIT;

    abAppend(&ab, "\x1b[?25l", 6);
    abAppend(&ab, "\x1b[H", 3);

    editorDrawRows(editor, &ab);
    editorDrawStatusBar(editor, &ab);
    editorDrawMessageBar(editor, &ab);

    char buf[32];
    snprintf(buf, sizeof(buf), "\x1b[%d;%dH",
             editor->cy - editor->rowoff + 1,
             editor->rx - editor->coloff + 1 +
                 editor->num_width + 1);
    abAppend(&ab, buf, strlen(buf));

    abAppend(&ab, "\x1b[?25h", 6);

    write(STDOUT_FILENO, ab.b, ab.len);
    abFree(&ab);
}

void editorSetStatusMessage(struct editorConfig* editor,
                            const char* fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    vsnprintf(editor->statusmsg, sizeof(editor->statusmsg),
              fmt, ap);
    va_end(ap);
    editor->statusmsg_time = time(NULL);
}