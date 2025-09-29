#ifndef DATA_H
#define DATA_H

#include <termios.h>
#include <time.h>

/*** data ***/

typedef struct erow
{
    int idx;
    int size;
    int rsize;
    char* chars;
    char* render;
    unsigned char* hl;
    int hl_open_comment;
} erow;

struct editorConfig
{
    int cx, cy;
    int rx;
    int rowoff;
    int coloff;
    int screenrows;
    int screencols;
    int numrows;
    erow* row;
    int num_width;
    int dirty;
    char* filename;
    char statusmsg[80];
    time_t statusmsg_time;
    struct editorSyntax* syntax;
    struct termios orig_termios;
};

extern struct editorConfig Editors[];

/*** defines ***/

#define CTRL_KEY(k) ((k) & 0x1f)

enum editorKey
{
    BACKSPACE = 127,
    ARROW_LEFT = 1000,
    ARROW_RIGHT,
    ARROW_UP,
    ARROW_DOWN,
    DEL_KEY,
    HOME_KEY,
    END_KEY,
    PAGE_UP,
    PAGE_DOWN
};


#endif