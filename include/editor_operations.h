#ifndef EDITOR_OPERATIONS_H
#define EDITOR_OPERATIONS_H

#include "data.h"

void editorInsertChar(struct editorConfig* editor, int c);
void editorDelChar(struct editorConfig* editor);
void editorInsertNewLine(struct editorConfig* editor);

#endif