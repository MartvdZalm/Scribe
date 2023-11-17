#ifndef ROW_H
#define ROW_H

#include <string>

class Row
{
private:
	std::string chars;
	std::string render;

public:
	Row();

	~Row();

	int getSize();

	int getRenderSize();

	std::string& getChars();

	void setChars(std::string chars);

	std::string& getRender();

	void setRender(std::string render);

};

#endif