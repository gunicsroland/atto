#ifndef EDITOR_O_H
#define EDITOR_O_H

#include "append_buffer.h"

void editorScroll();
void editorDrawRows(struct abuf* ab);
void editorDrawStatusBar(struct abuf* ab);
void editorDrawMessageBar(struct abuf* ab);
void editorRefreshScreen();
void editorSetStatusMessage(const char* fmt, ...);

#endif
