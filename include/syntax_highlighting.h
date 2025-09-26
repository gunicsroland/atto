#ifndef SYNTAX_HIGHLIGHTING_H
#define SYNTAX_HIGHLIGHTING_H

#include "data.h"

int is_separator(int c);
void editorUpdateSyntax(erow *row);
void editorSelectSyntaxHighlight();


#endif
