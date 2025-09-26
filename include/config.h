#ifndef CONFIG_H
#define CONFIG_H

#include <stddef.h>

/*** defines ***/

#define ATTO_VERSION "0.2.4"
#define ATTO_TAB_STOP 8
#define ATTO_QUIT_TIMES 2

#define HL_HIGHLIGHT_NUMBERS (1 << 0)
#define HL_HIGHLIGHT_STRINGS (1 << 1)

// colors for the different highlights
//  based on ansii escape sequences

enum editorHighlight
{
    HL_NORMAL = 7,
    HL_COMMENT = 6,
    HL_MLCOMMENT = 6,
    HL_KEYWORD1 = 3,
    HL_KEYWORD2 = 2,
    HL_STRING = 5,
    HL_NUMBER = 1,
    HL_MATCH = 4
};

struct editorSyntax
{
    const char* filetype;
    const char** filematch;
    const char** keywords;
    const char* singleline_comment_start;
    const char* multiline_comment_start;
    const char* multiline_comment_end;
    const int flags;
};

/*** filetypes ***/

extern const char* C_HL_extensions[];
extern const char* C_HL_keywords[];
extern const char* CPP_HL_extensions[];
extern const char* CPP_HL_keywords[];

extern struct editorSyntax HLDB[];

extern size_t HLDB_ENTRIES;

#endif
