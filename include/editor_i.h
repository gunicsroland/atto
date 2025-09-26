#ifndef EDITOR_I_H
#define EDITOR_I_H

char* editorPrompt(char* prompt, void (*callback)(char*, int));
void editorMoveCursor(int key);
void editorProcessKeypress();

#endif
