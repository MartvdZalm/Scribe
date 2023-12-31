#include "Editor/Row.h"

Row::Row() :
str("") {}

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

void Row::insert(int i, char c)
{
    if (str.length() == 0) {
        str.push_back(c);
    } else {
        str.insert(i, 1, c);
    }
}