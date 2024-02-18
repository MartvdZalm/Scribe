#ifndef ROW_H
#define ROW_H

#include <string>

class Row
{
public:
	std::string str;

	Row();

	~Row();

	int getSize();

	std::string& getString();

	void setString(std::string str);

	void insertChar(int i, char c);

	void insertString(int i, std::string input);
};

#endif