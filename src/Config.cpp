#include "Config.hpp"

Config::Config() : 
x(0), 
y(0),
rowOff(0),
colOff(0),
screenRows(0),
screenCols(0) {}

Config::~Config() {}

int Config::getCursorPosition()
{
	char buf[32];
	int i = 0;

	if (write(STDOUT_FILENO, "\x1b[6n", 4) != 4) {
		return -1;
	}

	for (i = 0; i < sizeof(buf) - 1; i++) {
		if (read(STDIN_FILENO, &buf[i], 1) != 1) {
			break;
		}

		if (buf[i] == 'R') {
			break;
		}
	}

	buf[i] = '\0';

	if (buf[0] != '\x1b' || buf[1] != '[') {
		return -1;
	}

	if (sscanf(&buf[2], "%d;%d", &screenRows, &screenCols) != 2) {
		return -1;
	}

	return 0;
}

int Config::getWindowSize()
{
	winsize ws;

	if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1 || ws.ws_col == 0) {
		if (write(STDOUT_FILENO, "\x1b[999C\x1b[999B", 12) != 12) {
 			return -1;
		}

		return getCursorPosition();
	}

	screenCols = ws.ws_col;
	screenRows = ws.ws_row;

	return 0;
}
