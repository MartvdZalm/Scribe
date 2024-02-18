#ifndef EDITOR_H
#define EDITOR_H

#include <vector>
#include <fstream>
#include <filesystem>
#include <cstring>
#include <termios.h>
#include <time.h>

#include "editor/terminal/Terminal.hpp"
#include "editor/exceptions/Exceptions.hpp"
#include "editor/keybindings/Key.hpp"
#include "editor/highlighting/Highlighting.hpp"
#include "editor/Row.hpp"
#include "editor/Config.hpp"

#define SCRIBE_VERSION "0.0.1"
#define SCRIBE_TAB_STOP 4
#define SCRIBE_QUIT_TIMES 2

#define CTRL_KEY(k) ((k) & 0x1f)

class Editor
{
public:	
	Config config;
	Terminal terminal;
	Exceptions exception;
	Highlighting highlighting;

	int dirty;
	std::string filename;
	std::string statusMessage;
	time_t statusMessageTime;
	std::vector<Row> rows;

	Editor();
	~Editor();

	void open(char *filename);
	void addRow(int index, std::string str);
	int readKey();
	void refreshScreen();
	void scroll();
	void drawRows(std::string *ab);
	void drawStatusBar(std::string *ab);
	void drawMessageBar(std::string *ab);
	void moveCursor(int key);
	void insertChar(int c);
	void insertNewline();
	void deleteChar();
	void deleteRow(int index);
	void find();
	void processKeypress();
	void save();
	void setStatusMessage(std::string message);
	void updateSyntax(Row* row);

	std::string prompt(std::string prompt, void (*callback)(std::vector<Row> rows, Config& config, std::string, int));
};

#endif