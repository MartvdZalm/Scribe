#ifndef ROW_H
#define ROW_H

#include <string>

class Row
{
private:
	int size;
	std::string chars;

public:
	int& getSize();

	void setSize(int size);

	std::string& getChars();

	void setChars(std::string chars);

};

#endif