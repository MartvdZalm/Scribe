#include <unistd.h>
#include <termios.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <cctype>
#include <time.h>

#include <Config.h>
#include <Key.h>
#include <Row.h>

#define SCRIBE_VERSION "0.0.1"
#define SCRIBE_TAB_STOP 4
#define SCRIBE_QUIT_TIMES 2

#define CTRL_KEY(k) ((k) & 0x1f)

Config config = Config();

void die(const char *s)
{
	write(STDOUT_FILENO, "\x1b[2J", 4);
	write(STDOUT_FILENO, "\x1b[H", 3);

	perror(s);
	exit(1);
}

void disableRawMode()
{
	if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &config.getOrigTermios()) == -1)
		die("tcsetattr");
}

void enableRawMode()
{
  	if (tcgetattr(STDIN_FILENO, &config.getOrigTermios()) == -1) die("tcgetattr");
  	atexit(disableRawMode);

  	termios raw = config.getOrigTermios();
  	raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
  	raw.c_oflag &= ~(OPOST);
  	raw.c_cflag |= ~(CS8);
  	raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
  	raw.c_cc[VMIN] = 0;
  	raw.c_cc[VTIME] = 1;

  	if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) die("tcsetattr");
}

int editorReadKey()
{
	int nread;
	char c;

	while ((nread = read(STDIN_FILENO, &c, 1)) != 1) {
		if (nread == -1 && errno != EAGAIN) die("read");
	}

	if (c == '\x1b') {
		char seq[3];

		if (read(STDIN_FILENO, &seq[0], 1) != 1) return '\x1b';
		if (read(STDIN_FILENO, &seq[1], 1) != 1) return '\x1b';

		if (seq[0] == '[') {

			if (seq[1] >= '0' && seq[1] <= '9') {

				if (read(STDIN_FILENO, &seq[2], 1) != 1) return '\x1b';
				if (seq[2] == '~') {
					switch (seq[1]) {
					case '1': return HOME_KEY;
					case '3': return DEL_KEY;
					case '4': return END_KEY;
					case '5': return PAGE_UP;
					case '6': return PAGE_DOWN;
					case '7': return HOME_KEY;
					case '8': return END_KEY;
					}
				}

			} else {
				switch (seq[1]) {
				case 'A': return ARROW_UP;
				case 'B': return ARROW_DOWN;
				case 'C': return ARROW_RIGHT;
				case 'D': return ARROW_LEFT;
				case 'H': return HOME_KEY;
				case 'F': return END_KEY;
				}
			}
		} else if (seq[0] == '0') {
			switch (seq[1]) {
			case 'H': return HOME_KEY;
			case 'F': return END_KEY;
			}
		}

		return '\x1b';
	} else {
		return c;
	}
}

int getCursorPosition(int *rows, int *cols)
{
	char buf[32];
	uint i = 0;

	if (write(STDOUT_FILENO, "\x1b[6n", 4) != 4) return -1;

	while (i < sizeof(buf) - 1) {
		if (read(STDIN_FILENO, &buf[i], 1) != 1) break;
		if (buf[i] == 'R') break;
		i++;
	}
	buf[i] = '\0';

	if (buf[0] != '\x1b' || buf[1] != '[') return -1;
	if (sscanf(&buf[2], "%d;%d", rows, cols) != 2) return -1;

	return 0;
}

int getWindowSize(int *rows, int *cols)
{
	winsize ws;

	if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1 || ws.ws_col == 0) {
		if (write(STDOUT_FILENO, "\x1b[999C\x1b[999B", 12) != 12) return -1;
		return getCursorPosition(rows, cols);
	} else {
		*cols = ws.ws_col;
		*rows = ws.ws_row;
		return 0;
	}
}

void editorOpen(char *filename)
{
	config.setFilename(filename);

	std::ifstream file(filename);

	if (!file.is_open()) {
        throw std::runtime_error("Failed to open file");
	}

    std::string line;
    while (std::getline(file, line)) {

    	line.erase(std::find_if(line.rbegin(), line.rend(), [](int ch) {
        	return !std::isspace(ch);
    	}).base(), line.end());

    	config.addRow(config.getNumRows(), line);
    }

    file.close();
}

void editorScroll()
{
	if (config.getCoordinateY() < config.getRowOff()) {
		config.setRowOff(config.getCoordinateY());
	}

	if (config.getCoordinateY() >= config.getRowOff() + config.getScreenRows()) {
		config.setRowOff(config.getCoordinateY() - config.getScreenRows() + 1);
	}

	if (config.getCoordinateX() < config.getColOff()) {
		config.setColOff(config.getCoordinateX());
	}

	if (config.getCoordinateX() >= config.getColOff() + config.getScreenCols()) {
		config.setColOff(config.getCoordinateX() - config.getScreenCols() + 1);
	}
}

void editorDrawRows(std::string *ab)
{
	for (int i = 0; i < config.getScreenRows(); i++) {

		int fileRow = i + config.getRowOff();

		if (fileRow >= config.getNumRows()) {

			if (config.getNumRows() == 0 && i == config.getScreenRows() / 3) {
				std::string greeting = "Scribe Editor -- version " + std::string(SCRIBE_VERSION);

				int padding = (config.getScreenCols() - greeting.length()) / 2;
				if (padding) {
					ab->append("~");
					padding--;
				}

				while (padding--) ab->append(" ");
				ab->append(greeting);

			} else {
				ab->append("~");
			}

		} else {
			int len = config.getRowAt(fileRow).getSize() - config.getColOff();
			if (len < 0) len = 0;
			if (len > config.getScreenCols()) len = config.getScreenCols();
			ab->append(config.getRowAt(fileRow).getString().substr(config.getColOff(), len));
		}

		ab->append("\x1b[K");
		ab->append("\r\n");
	}
}

void editorDrawStatusBar(std::string *ab)
{
	ab->append("\x1b[7m");

	std::string status = config.getFilename().substr(0, 20) + " - " + 
	std::to_string(config.getNumRows()) + " lines " +
	(config.getDirty() ? "(modified)" : "");

	ab->append(status);

    int availableSpace = config.getScreenCols() - (status.length() + 1);

    if (availableSpace >= static_cast<int>(std::to_string(config.getCoordinateY() + 1).length())) {
        while (availableSpace > static_cast<int>(std::to_string(config.getCoordinateY() + 1).length())) {
            ab->append(" ");
            availableSpace--;
        }
    } 

	ab->append(std::to_string(config.getCoordinateY() + 1));
  	ab->append("\x1b[m");
	ab->append("\r\n");
}

void editorDrawMessageBar(std::string *ab)
{
	ab->append("\x1b[K");

	if (time(NULL) - config.getStatusMessageTime() < 5) {
		ab->append(config.getStatusMessage());
	}
}

void editorRefreshScreen()
{
	editorScroll();

	std::string ab;

	ab.append("\x1b[?25l");
	ab.append("\x1b[H");

	editorDrawRows(&ab);
	editorDrawStatusBar(&ab);
	editorDrawMessageBar(&ab);

	ab.append("\x1b[" + std::to_string((config.getCoordinateY() - config.getRowOff()) + 1) + ";" + std::to_string((config.getCoordinateX() - config.getColOff()) + 1) + "H");
	ab.append("\x1b[?25h");

	write(STDOUT_FILENO, ab.c_str(), ab.length());
}

void editorMoveCursor(int key)
{
	Row *row = (config.getCoordinateY() >= config.getNumRows()) ? NULL : &config.getRowAt(config.getCoordinateY());

	switch (key) {

	case ARROW_LEFT:
		if (config.getCoordinateX() != 0) {
			config.decCoordinateX();
		} else if (config.getCoordinateY() > 0) {
			config.decCoordinateY();
			config.setCoordinateX(config.getRowAt(config.getCoordinateY()).getSize());
		}
		break;

	case ARROW_RIGHT:
		if (row && config.getCoordinateX() < row->getSize()) {
			config.incCoordinateX();
		} else if (row && config.getCoordinateX() == row->getSize()) {
			config.incCoordinateY();
			config.setCoordinateX(0);
		}
		break;

	case ARROW_UP:
		if (config.getCoordinateY() != 0) {
			config.decCoordinateY();
		}
		break;

	case ARROW_DOWN:
		if (config.getCoordinateY() < config.getNumRows()) {
			config.incCoordinateY();
		}
		break;
	}

	row = (config.getCoordinateY() >= config.getNumRows()) ? NULL : &config.getRowAt(config.getCoordinateY());
	int rowLen = row ? row->getSize() : 0;

	if (config.getCoordinateX() > rowLen) {
		config.setCoordinateX(rowLen);
	}
}

void editorInsertChar(int c)
{
	if (config.getCoordinateY() == config.getNumRows()) {
		config.addRow(config.getNumRows(), "");
	}

	Row *row = &config.getRowAt(config.getCoordinateY());
	int index = config.getCoordinateX();

	if (index < 0 || index > row->getSize()) {
		index = row->getSize();
	}

	row->insertChar(index, c);

	config.incCoordinateX();	
	config.incDirty();
}

void editorInsertNewline()
{
	if (config.getCoordinateX() == 0) {
		config.addRow(config.getCoordinateY(), "");
	} else {
		Row *row = &config.getRowAt(config.getCoordinateY());

		std::string substring = row->getString().substr(config.getCoordinateX());
		row->getString().erase(config.getCoordinateX());
		config.addRow(config.getCoordinateY() + 1, substring);
	}

	config.incCoordinateY();
	config.setCoordinateX(0);
}

void editorDeleteChar()
{
	if (config.getCoordinateY() == config.getNumRows()) {
		return;
	}

	if (config.getCoordinateX() == 0 && config.getCoordinateY() == 0) {
		return;
	}

	Row *row = &config.getRowAt(config.getCoordinateY());
  		
	if (config.getCoordinateX() > 0) {
		int index = config.getCoordinateX() - 1;

		if (index < 0 || index > row->getSize()) {
			return;
		}

		row->getString().erase(index, 1);
		config.incDirty();

		config.decCoordinateX();
	} else {
		config.setCoordinateX(config.getRowAt(config.getCoordinateY() - 1).getSize());
		config.deleteRow(config.getCoordinateY());

		config.decCoordinateY();
	}
}

std::string editorPrompt(std::string prompt, void (*callback)(std::string, int))
{
	std::string input;

	while (1) {
		config.setStatusMessage(prompt);
		editorRefreshScreen();

		int c = editorReadKey();

		if (c == DEL_KEY || c == CTRL_KEY('h') || c == BACKSPACE) {
			if (!input.empty()) {
		        input.pop_back();
		        prompt.pop_back();
		    }
		} else if (c == '\x1b') {
			config.setStatusMessage("");
			if (callback) callback(input, c);
			return "";
		} else if (c == '\r') {
			if (!input.empty()) {
				config.setStatusMessage("");
				if (callback) callback(input, c);
				return input;
			}
		} else if (!iscntrl(c) && c < 128) {
			input.push_back(c);
			prompt.push_back(c);
		}

		if (callback) callback(input, c);
	}
}

void editorFindCallback(std::string query, int key)
{
	static int lastMatch = -1;
	static int direction = 1;

	if (key == '\r' || key == '\x1b') {
		lastMatch = -1;
		direction = 1;
		return;
	} else if (key == ARROW_RIGHT || key == ARROW_DOWN) {
		direction = 1;
	} else if (key == ARROW_LEFT || key == ARROW_UP) {
		direction = -1;
	} else {
		lastMatch = -1;
		direction = 1;
	}

	if (lastMatch == -1) {
		direction = 1;
	}

	int current = lastMatch;

	for (int i = 0; i < config.getNumRows(); i++) {
		current += direction;

		if (current == -1) {
			current = config.getNumRows() - 1;
		} else if (current == config.getNumRows()) {
			current = 0;
		}

		Row *row = &config.getRowAt(current);
		std::string rowString = row->getString();

		size_t position = rowString.find(query);

		if (position != std::string::npos) {
			lastMatch = current;
			config.setCoordinateY(current);
			config.setCoordinateX(position);
			config.setRowOff(config.getNumRows());
			break;
        }
	}
}

void editorFind()
{
	int savedCx = config.getCoordinateX();
	int savedCy = config.getCoordinateY();
	int savedRowOff = config.getRowOff();
	int savedColOff = config.getColOff();

	std::string query = editorPrompt("Search: ", editorFindCallback);

	if (query.empty()) {
		config.setCoordinateX(savedCx);
		config.setCoordinateY(savedCy);
		config.setRowOff(savedRowOff);
		config.setColOff(savedColOff);
	}
}

void editorProcessKeypress()
{
	static int quitTimes = SCRIBE_QUIT_TIMES;
	int c = editorReadKey();

	switch (c) {

	case '\r':
		editorInsertNewline();
		break;

	case CTRL_KEY('q'):
		if (config.getDirty() != 0 && quitTimes > 0) {
	        config.setStatusMessage("WARNING!!! File has unsaved changes. Press Ctrl-Q " + std::to_string(quitTimes) + " more times to quit.");
	        quitTimes--;
	        return;
      	}

		write(STDOUT_FILENO, "\x1b[2J", 4);
		write(STDOUT_FILENO, "\x1b[H", 3);
		exit(0);
		break;

	case CTRL_KEY('s'):

		if (config.getFilename() == "[No Name]") {
			std::string filename = editorPrompt("Save as: ", NULL);

			if (!filename.empty()) {
				config.setFilename(filename);
			} else {
				config.setStatusMessage("Save aborted");
				return;
			}
		}

		config.saveRows();
		break;

	case HOME_KEY:
		config.setCoordinateX(0);
		break;

	case END_KEY:
		config.setCoordinateX(config.getScreenCols() - 1);
		break;

	case CTRL_KEY('f'):
		editorFind();
		break;

	case BACKSPACE:
	case CTRL_KEY('h'):
	case DEL_KEY:
     	if (c == DEL_KEY) editorMoveCursor(ARROW_RIGHT);
      	editorDeleteChar();
		break;

	case PAGE_UP:
	case PAGE_DOWN: 
		{
			int times = config.getScreenRows();
			while (times--) {
				editorMoveCursor(c == PAGE_UP ? ARROW_UP : ARROW_DOWN);
			}
		}
		break;

	case ARROW_UP:
	case ARROW_DOWN:
	case ARROW_LEFT:
	case ARROW_RIGHT:
		editorMoveCursor(c);
		break;

	case CTRL_KEY('l'):
	case '\x1b':
		break;

	default:
		editorInsertChar(c);
		break;
	}

	quitTimes = SCRIBE_QUIT_TIMES;
}

void initEditor()
{
	if (getWindowSize(&config.getScreenRows(), &config.getScreenCols()) == -1) die("getWindowSize");
	config.setScreenRows(config.getScreenRows() - 2);
}

int main(int argc, char *argv[])
{
	enableRawMode();
	initEditor();

	if (argc >= 2) {
		editorOpen(argv[1]);
	}

	config.setStatusMessage("HELP: Ctrl-S = save | Ctrl-Q = quit | Ctrl-F = find");

  	while (1) {
  		editorRefreshScreen();
	  	editorProcessKeypress();
  	}

  	return 0;
}
