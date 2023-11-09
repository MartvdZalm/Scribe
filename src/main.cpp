#include <unistd.h>
#include <termios.h>
#include <stdlib.h>

struct termios original_termios;

void disableRawMode()
{
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &original_termios);
}

void enableRawMode()
{
  	tcgetattr(STDIN_FILENO, &original_termios);
  	atexit(disableRawMode);

  	struct termios raw = original_termios;

  	raw.c_lflag &= ~(ECHO);

  	tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

int main()
{
	enableRawMode();

  	char c;
  	while (read(STDIN_FILENO, &c, 1) == 1 && c != 'q');

  	return 0;
}