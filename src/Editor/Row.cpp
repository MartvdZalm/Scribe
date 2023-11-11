#include "Editor/Row.h"

int& Row::getSize()
{
	return size;
}

void Row::setSize(int s)
{
	size = s;
}

std::string& Row::getChars()
{
	return chars;
}

void Row::setChars(std::string c)
{
	chars = c;
}