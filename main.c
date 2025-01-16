/*** includes ***/

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <errno.h>
#include <unistd.h>
#include <termios.h>

/*** data ***/

struct termios orig_termios;

/*** terminal ***/

void die(const char *s){
	perror(s);
	exit(1);
}

void disableRawMode() {
	if(tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios) == -1)
		die("tcsettattr");
}

void enableRawMode() {
	struct termios raw;

	if(tcgetattr(STDIN_FILENO, &orig_termios) == -1)
		die("tcgetattr");
	atexit(disableRawMode);

	raw = orig_termios;

	raw.c_iflag &= ~(ICRNL | IXON | BRKINT | INPCK | ISTRIP);
	raw.c_oflag &= ~(OPOST);
	raw.c_cflag |= (CS8);
	raw.c_lflag &= ~(ECHO | ICANON);
	raw.c_cc[VMIN] = 0;
	raw.c_cc[VTIME] = 1;

	if(tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1)
		die("tcsetattr");
}

/*** init ***/

int main() {
	enableRawMode();	

	char c;
	while (1){
		c = '\0';
		if(read(STDIN_FILENO, &c, 1) == -1 && errno != EAGAIN)
			die("read");
		if(iscntrl(c)){
			printf("%d\r\n", c);
		}
		else{
			printf("%d ('%c')\r\n", c, c);
		}
		if(c == 'q') break;
	}
	return 0;
}