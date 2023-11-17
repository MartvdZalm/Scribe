#include "Editor/Row.h"

Row::Row() {}

Row::~Row() {}

int Row::getSize()
{
    int rowSize = 0;

    for (char c : str) {

        if (c == '\t') {
            rowSize += (8 - (rowSize % 8));
        } else {
            rowSize++;
        }
    }

    return rowSize;
}

std::string& Row::getString()
{
	return str;
}

void Row::setString(std::string str)
{
	this->str = str;
}