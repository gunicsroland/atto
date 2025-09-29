#ifndef SYNTAX_HIGHLIGHTING_H
#define SYNTAX_HIGHLIGHTING_H

#include "data.h"

int is_separator(int c);
void editorUpdateSyntax(struct editorConfig* editor, erow* row);
void editorSelectSyntaxHighlight(struct editorConfig* editor);

#endif
