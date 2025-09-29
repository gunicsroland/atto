#ifndef ROW_OPERATIONS_H
#define ROW_OPERATIONS_H

#include "data.h"

int editorGetNumLen(int num);
int editorRowCxToRx(erow* row, int cx);
int editorRowRxToCx(erow* row, int rx);
void editorUpdateRow(struct editorConfig* editor, erow* row);
void editorInsertRow(struct editorConfig* editor, int at, char* s, size_t len);
void editorFreeRow(erow* row);
void editorDelRow(struct editorConfig* editor, int at);
void editorRowInsertChar(struct editorConfig* editor, erow* row, int at, int c);
void editorRowAppendString(struct editorConfig* editor, erow* row, char* s, size_t len);
void editorRowDelChar(struct editorConfig* editor, erow* row, int at);

#endif
