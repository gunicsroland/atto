#include "../include/data.h"
#include "../include/config.h"
#include "../include/editor_i.h"
#include "../include/editor_o.h"
#include "../include/file_io.h"
#include "../include/terminal.h"

#include <stdio.h>

struct editorConfig Editors[ATTO_MAX_BUFFERS];
int editor_num = 0;
int current_editor = 0;

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

    if (getWindowSize(&editor->screenrows, &editor->screencols) == -1)
        die("getWindowSize");
    editor->screenrows -= 2;
    editor->screencols /= ATTO_MAX_BUFFERS;

    editor_num++;
}

int main(int argc, char* argv[])
{
    enableRawMode();
    
    for(int i = 0; i < argc && i < ATTO_MAX_BUFFERS; ++i){
        initEditor(&Editors[i]);
        editorOpen(&Editors[i], argv[i+1]);
    }

    if(editor_num == 0){
        initEditor(&Editors[0]);
    }

    printf("Entering main loop\n");
    for(int i = 0; i < editor_num; ++i){
        editorRefreshScreen(&Editors[i]);
        editorSetStatusMessage(&Editors[0], "^Q - quit  ^S - Save  ^F - Find  ^L - Line  ^J ^K - Move tabs");
    }

    while (1)
    {
        editorRefreshScreen(&Editors[current_editor]);
        editorProcessKeypress(&Editors[current_editor]);
    }
    return 0;
}