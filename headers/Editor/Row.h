#ifndef ROW_H
#define ROW_H

#include <string>

class Row
{
private:
	std::string str;

public:
	Row();

	~Row();

	int getSize();

	std::string& getString();

	void setString(std::string str);

	void insert(int i, char c);
};

#endif