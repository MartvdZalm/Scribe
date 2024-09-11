#include "editor/Row.hpp"

Row::Row() :
str("") {}

Row::~Row() {}

int Row::getSize()
{
    return str.length();
}

std::string& Row::getString()
{
	return str;
}

void Row::setString(std::string str)
{
	this->str = str;
}

void Row::insertChar(int i, char c)
{
    if (str.length() == 0) {
        str.push_back(c);
    } else {
        str.insert(i, 1, c);
    }
}

void Row::insertString(int i, std::string input)
{
    if (str.length() == 0) {
        str.append(input);
    } else {
        str.insert(str.begin() + i, input.begin(), input.end());
    }
}