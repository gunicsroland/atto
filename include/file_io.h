#ifndef FILE_IO_H
#define FILE_IO_H

#include "data.h"

char* editorRowsToString(struct editorConfig* editor,
                         int* buflen);
void editorOpen(struct editorConfig* editor,
                char* filename);
void editorSave(struct editorConfig* editor);

#endif