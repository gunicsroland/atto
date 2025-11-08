#include "../include/config.h"

const char* const c_hl_extensions[] = {".c", ".h", NULL};

const char* const c_hl_keywords[] = {
    "switch", "if",       "do",        "while",   "for",
    "break",  "continue", "return",    "else",    "struct",
    "union",  "typedef",  "static",    "enum",    "case",
    "void",   "const",

    "int|",   "union|",   "long|",     "double|", "float|",
    "char|",  "bool|",    "unsigned|", "signed|", "coid|",
    NULL};

const char* const cpp_hl_extensions[] = {
    ".cpp", ".cc", ".cxx", ".c++", ".C", ".hpp",
    ".h",   ".hh", ".hxx", ".h++", ".H", NULL};

const char* const cpp_hl_keywords[] = {
    "switch",   "if",       "do",       "while",
    "for",      "break",    "continue", "return",
    "else",     "struct",   "typedef",  "static",
    "enum",     "class",    "case",     "union",
    "void",     "try",      "catch",    "const",
    "friend",   "public",   "private",  "protected",
    "template", "operator",

    "int|",     "long|",    "double|",  "float|",
    "char|",    "union",    "bool|",    "unsigned|",
    "signed|",  "coid|",    NULL};

struct editorSyntax hldb[] = {
    {"c", c_hl_extensions, c_hl_keywords, "//", "/*", "*/",
     HL_HIGHLIGHT_NUMBERS | HL_HIGHLIGHT_STRINGS},
    {"cpp", cpp_hl_extensions, cpp_hl_keywords, "//", "/*",
     "*/", HL_HIGHLIGHT_NUMBERS | HL_HIGHLIGHT_STRINGS}};

size_t hldb_entries = sizeof(hldb) / sizeof(hldb[0]);