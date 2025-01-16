#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <unistd.h>
#include <termios.h>

struct termios orig_termios;

void disableRawMode() {
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
}

void enableRawMode() {
	struct termios raw;

	tcgetattr(STDIN_FILENO, &orig_termios);
	atexit(disableRawMode);

	raw = orig_termios;

	raw.c_iflag &= ~(ICRNL | IXON | BREAKINT | INPCK | ISTRIP);
	raw.c_oflag &= ~(OPOST);
	raw.c_cflag |= (CS8);
	raw.c_lflag &= ~(ECHO | ICANON);

	tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

int main() {
	enableRawMode();	

	char c;
	while (read(STDIN_FILENO, &c, 1) == 1 && c != 'q'){
		if(iscntrl(c)){
			printf("%d\r\n", c);
		}
		else{
			printf("%d ('%c')\r\n", c, c);
		}
	}
	return 0;
}