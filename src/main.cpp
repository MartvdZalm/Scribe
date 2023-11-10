#include <unistd.h>
#include <termios.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>
#include <errno.h>

#define CTRL_KEY(k) ((k) & 0x1f)

struct termios original_termios;

void die(const char *s)
{
	perror(s);
	exit(1);
}

void disableRawMode()
{
	if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &original_termios) == -1)
		die("tcsetattr");
}

void enableRawMode()
{
  	if (tcgetattr(STDIN_FILENO, &original_termios) == -1) die("tcgetattr");
  	atexit(disableRawMode);

  	struct termios raw = original_termios;
  	raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
  	raw.c_iflag &= ~(OPOST);
  	raw.c_iflag |= ~(CS8);
  	raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
  	raw.c_cc[VMIN] = 0;
  	raw.c_cc[VTIME] = 1;

  	if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) die("tcsetattr");
}

char editorReadKey()
{
	int nread;
	char c;

	while ((nread = read(STDIN_FILENO, &c, 1)) != 1) {
		if (nread == -1 && errno != EAGAIN) die("read");
	}
	
	return c;
}

int main()
{
	enableRawMode();

  	while (1) {
	    char c = '\0';

	    if (read(STDIN_FILENO, &c, 1) == -1 && errno != EAGAIN) die("read");

	    if (iscntrl(c)) {
	      printf("%d\r\n", c);
	    } else {
	      printf("%d ('%c')\r\n", c, c);
	    }

	    if (c == CTRL_KEY('q')) break;
  	}

  	return 0;
}