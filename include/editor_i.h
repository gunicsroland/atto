#ifndef EDITOR_I_H
#define EDITOR_I_H

#include "data.h"

char* editorPrompt(char* prompt, void (*callback)(struct editorConfig*, char*, int));
void editorMoveCursor(struct editorConfig* editor, int key);
void editorProcessKeypress(struct editorConfig* editor);

#endif
