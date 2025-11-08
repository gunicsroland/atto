#ifndef EDITOR_O_H
#define EDITOR_O_H

#include "append_buffer.h"
#include "data.h"

// TODO

void editorScroll(struct editorConfig* editor);
void editorDrawRows(struct editorConfig* editor,
                    struct abuf* ab);
void editorDrawStatusBar(struct editorConfig* editor,
                         struct abuf* ab);
void editorDrawMessageBar(struct editorConfig* editor,
                          struct abuf* ab);
void editorRefreshScreen(struct editorConfig* editor);
void editorSetStatusMessage(struct editorConfig* editor,
                            const char* fmt, ...);

#endif
