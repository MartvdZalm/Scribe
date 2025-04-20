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
#include "editor/SyntaxHighlighter.hpp"
#include "editor/keybindings/Key.hpp"
#include "editor/Row.hpp"
#include "editor/Config.hpp"
#include "editor/Highlighting.hpp"
#include "editor/Preferences.hpp"

#define SCRIBE_VERSION "0.0.1"
#define SCRIBE_TAB_STOP 4
#define SCRIBE_QUIT_TIMES 2

#define CTRL_KEY(k) ((k) & 0x1f)

class Editor
{
public:	
	Editor();
	~Editor();

	void open(const std::string& filename);
	void addRow(int index, std::string str);
	void setPreferences(const Preferences* pref);


	int readKey();
	void refreshScreen();
	void scroll();
	void drawRows(std::string *ab);
	void applyHighlighting(Row* row, HighlightTheme theme, std::string* ab);
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

	std::string prompt(std::string prompt, void (*callback)(std::vector<Row> rows, Config& config, std::string, int));

private:
	Config config;
	Terminal terminal;
	Exceptions exception;
	SyntaxHighlighter highlighter;
	const Preferences* preferences;

	int dirty = 0;
	std::string filename = "[No Name]";
	std::string statusMessage = "\0";
	time_t statusMessageTime = 0;
	std::vector<Row> rows;

};

#endif
