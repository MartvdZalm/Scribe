#include "editor/exceptions/Exceptions.hpp"

Exceptions::Exceptions() {}
Exceptions::~Exceptions() {}

void Exceptions::die(const char *s)
{
	write(STDOUT_FILENO, "\x1b[2J", 4);
	write(STDOUT_FILENO, "\x1b[H", 3);

	perror(s);
	exit(1);
}