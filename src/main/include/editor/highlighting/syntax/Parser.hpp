#ifndef PARSER_H
#define PARSER_H	

#include <string>
#include <iostream>
#include <fstream>

#include "editor/exceptions/Exception.h"

class Parser
{	
	std::ifstream content;
	Exception exception;

	Parser(std::string syntaxName);
	~Parser();

	void parse();
};

#endif