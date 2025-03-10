#ifndef CONFIG_H
#define CONFIG_H

/*** defines ***/

#define ATTO_VERSION "0.2.3"
#define ATTO_TAB_STOP 8
#define ATTO_QUIT_TIMES 2

#define HL_HIGHLIGHT_NUMBERS (1 << 0)
#define HL_HIGHLIGHT_STRINGS (1 << 1)

//colors for the different highlights
// based on ansii escape sequences

enum editorHighlight {
  HL_NORMAL = 7,
  HL_COMMENT = 6,
  HL_MLCOMMENT = 6,
  HL_KEYWORD1 = 3,
  HL_KEYWORD2 = 2,
  HL_STRING = 5,
  HL_NUMBER = 1,
  HL_MATCH = 4
};

struct editorSyntax {
  char *filetype;
  char **filematch;
  char **keywords;
  char *singleline_comment_start;
  char *multiline_comment_start;
  char *multiline_comment_end;
  int flags;
};


/*** filetypes ***/

char *C_HL_extensions[] = {".c", ".h", NULL};

char *C_HL_keywords[] = {"switch",
                         "if",
			 "do",
                         "while",
                         "for",
                         "break",
                         "continue",
                         "return",
                         "else",
                         "struct",
                         "union",
                         "typedef",
                         "static",
                         "enum",
                         "case",
			 "void",
			 "const",

                         "int|",
			 "union|",
                         "long|",
                         "double|",
                         "float|",
                         "char|",
			 "bool|",
                         "unsigned|",
                         "signed|",
                         "coid|",
                         NULL};

char *CPP_HL_extensions[] = {".cpp", ".cc", ".cxx", ".c++", ".C", ".hpp", ".h", ".hh", ".hxx", ".h++", ".H", NULL};

char *CPP_HL_keywords[] = {"switch",
                         "if",
			 "do",
                         "while",
                         "for",
                         "break",
                         "continue",
                         "return",
                         "else",
                         "struct",
                         "typedef",
                         "static",
                         "enum",
                         "class",
                         "case",
			 "union",
			 "void",
			 "try",
			 "catch",
			 "const",
			 "friend",
			 "public",
			 "private",
			 "protected",
			 "template",
			

                         "int|",
                         "long|",
                         "double|",
                         "float|",
                         "char|",
                         "union",
			 "bool|",
                         "unsigned|",
                         "signed|",
                         "coid|",
                         NULL};

struct editorSyntax HLDB[] = {
    {"c", C_HL_extensions, C_HL_keywords, "//", "/*", "*/",
     HL_HIGHLIGHT_NUMBERS | HL_HIGHLIGHT_STRINGS},
    {"cpp", CPP_HL_extensions, CPP_HL_keywords, "//", "/*", "*/",
     HL_HIGHLIGHT_NUMBERS | HL_HIGHLIGHT_STRINGS}
};

#define HLDB_ENTRIES (sizeof(HLDB) / sizeof(HLDB[0]))

#endif