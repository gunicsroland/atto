#include "../include/config.h"

const char* C_HL_extensions[] = {".c", ".h", NULL};

const char* C_HL_keywords[] = {"switch",   "if",        "do",      "while",   "for",    "break",
                               "continue", "return",    "else",    "struct",  "union",  "typedef",
                               "static",   "enum",      "case",    "void",    "const",

                               "int|",     "union|",    "long|",   "double|", "float|", "char|",
                               "bool|",    "unsigned|", "signed|", "coid|",   NULL};

const char* CPP_HL_extensions[] = {".cpp", ".cc", ".cxx", ".c++", ".C", ".hpp",
                                   ".h",   ".hh", ".hxx", ".h++", ".H", NULL};

const char* CPP_HL_keywords[] = {
    "switch",    "if",      "do",        "while",    "for",      "break", "continue",
    "return",    "else",    "struct",    "typedef",  "static",   "enum",  "class",
    "case",      "union",   "void",      "try",      "catch",    "const", "friend",
    "public",    "private", "protected", "template", "operator",

    "int|",      "long|",   "double|",   "float|",   "char|",    "union", "bool|",
    "unsigned|", "signed|", "coid|",     NULL};

struct editorSyntax HLDB[] = {{"c", C_HL_extensions, C_HL_keywords, "//", "/*", "*/",
                               HL_HIGHLIGHT_NUMBERS | HL_HIGHLIGHT_STRINGS},
                              {"cpp", CPP_HL_extensions, CPP_HL_keywords, "//", "/*", "*/",
                               HL_HIGHLIGHT_NUMBERS | HL_HIGHLIGHT_STRINGS}};

size_t HLDB_ENTRIES = sizeof(HLDB) / sizeof(HLDB[0]);