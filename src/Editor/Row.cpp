#include "Editor/Row.h"

Row::Row() {}

Row::~Row() {}

int Row::getSize()
{
	return static_cast<int>(chars.length());
}

int Row::getRenderSize()
{
	return static_cast<int>(render.length());
}

std::string& Row::getChars()
{
	return chars;
}

void Row::setChars(std::string chars)
{
	this->chars = chars;
}

std::string& Row::getRender()
{
	return render;
}

void Row::setRender(std::string render)
{
	this->render = render; 
}