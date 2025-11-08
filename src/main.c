#include "../include/config.h"
#include "../include/data.h"
#include "../include/editor_i.h"
#include "../include/editor_o.h"
#include "../include/file_io.h"
#include "../include/terminal.h"

#include <stdio.h>

struct editorConfig Editors[ATTO_MAX_BUFFERS];
int editor_num = 0;
int current_editor = 0;
int editor_count = 0;

void initEditor(struct editorConfig* editor)
{
    editor->cx = 0;
    editor->cy = 0;
    editor->rx = 0;
    editor->rowoff = 0;
    editor->coloff = 0;
    editor->numrows = 0;
    editor->row = NULL;
    editor->num_width = 1;
    editor->dirty = 0;
    editor->filename = NULL;
    editor->statusmsg[0] = '\0';
    editor->statusmsg_time = 0;
    editor->syntax = NULL;

    if (getWindowSize(&editor->screenrows,
                      &editor->screencols) == -1)
        die("getWindowSize");

    editor->screenrows -= 2;
    editor->width /= editor_num > 1 ? editor_num : 1;
    printf(
        "Screen cols: %d\n",
        editor->width); // Debug line to check width value
    editor->start_x = editor_count * editor->width;
    printf(
        "Editor %d start_x: %d\n", editor_count,
        editor
            ->start_x); // Debug line to check start_x value
    editor_count++;
}

int main(int argc, char* argv[])
{
    printf("Starting atto editor\n");
    editor_num = argc - 1;

    printf("Opening editors\n");
    for (int i = 0; i < argc - 1; ++i)
    {
        printf("Initializing editor %d\n", i);
        initEditor(&Editors[i]);
        printf("Opening file: %s\n", argv[i + 1]);
        editorOpen(&Editors[i], argv[i + 1]);
    }

    printf("Total editors opened: %d\n", editor_num);
    if (editor_num == 0)
    {
        initEditor(&Editors[0]);
    }

    enableRawMode();

    for (int i = 0; i < editor_num; ++i)
    {
        editorRefreshScreen(&Editors[i]);
        editorSetStatusMessage(
            &Editors[0], "^Q - quit  ^S - Save  ^F - Find  "
                         "^L - Line  ^J ^K - Move tabs");
    }

    while (1)
    {
        editorRefreshScreen(&Editors[current_editor]);
        editorProcessKeypress(&Editors[current_editor]);
    }

    disableRawMode();
    return 0;
}