#include "Row.hpp"

Row::Row() :
str(""),
hl(nullptr) {}

Row::~Row() {
    if (hl) delete[] hl;
}

int Row::getSize()
{
    return str.length();
}

std::string& Row::getString()
{
	return str;
}

void Row::setString(const std::string& s)
{
	str = s;
    hl = new unsigned char[str.size()];
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
