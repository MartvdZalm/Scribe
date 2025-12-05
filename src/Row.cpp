#include "Row.hpp"
#include <algorithm>

Row::Row() : str(""), hl() {}

int Row::getSize() const
{
    return static_cast<int>(str.size());
}

std::string& Row::getString()
{
    return str;
}

const std::string& Row::getString() const
{
    return str;
}

void Row::setString(const std::string& s)
{
    str = s;
    hl.assign(str.size(), 0);
}

void Row::insertChar(int i, char c)
{
    if (i < 0 || i > static_cast<int>(str.size()))
        i = static_cast<int>(str.size());

    str.insert(i, 1, c);
    hl.insert(hl.begin() + i, 0);
}

void Row::insertString(int i, const std::string& input)
{
    if (i < 0 || i > static_cast<int>(str.size()))
        i = static_cast<int>(str.size());

    str.insert(i, input);
    hl.insert(hl.begin() + i, input.size(), 0);
}
