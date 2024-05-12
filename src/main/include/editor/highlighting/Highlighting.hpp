#ifndef HIGHLIGHTING_H
#define HIGHLIGHTING_H

#include "editor/highlighting/syntax/Parser.hpp"
#include "editor/Row.hpp"

class Highlighting
{
public:
	Highlighting(const std::string& filePath);
	~Highlighting();
	int checkRow(Row* row);

private:
	const std::string ANSI_COLOR_RED = "\033[31m";
	const std::string ANSI_COLOR_RESET = "\033[0m";
	
	Parser parser;
};

#endif