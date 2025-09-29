#ifndef FIND_H
#define FIND_H

#include "data.h"

void editorFindCallback(struct editorConfig* editor, char* query, int key);
void editorFind(struct editorConfig* editor);
void editorFindLine(struct editorConfig* editor);

#endif