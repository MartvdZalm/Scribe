#ifndef TERMINAL_H
#define TERMINAL_H

#include <termios.h>
#include "Editor/Exceptions.h"

class Terminal
{
public:
	termios originalTerminal;
	Exceptions exception;

	Terminal();
	~Terminal();

	void disableRawMode();
	void enableRawMode();
};

#endif