#include "Editor/Config.h"

Config::Config() : 
coordinateX(0), 
coordinateY(0),
rowOff(0),
colOff(0),
rows(),
origTermios() {}

Config::~Config() {}

termios& Config::getOrigTermios()
{
	return origTermios;
}

int& Config::getScreenRows()
{
	return screenRows;
}

int& Config::getScreenCols()
{
	return screenCols;
}

int& Config::getCoordinateX()
{
	return coordinateX;
}

int& Config::getCoordinateY()
{
	return coordinateY;
}

void Config::setCoordinateX(int x)
{
	coordinateX = x;
}

void Config::setCoordinateY(int y)
{
	coordinateY = y;
}

int& Config::getRowOff()
{
	return rowOff;
}

void Config::setRowOff(int off)
{
	rowOff = off;
}

int& Config::getColOff()
{
	return colOff;
}

void Config::setColOff(int off)
{
	colOff = off;
}

int Config::getNumRows()
{
	return rows.size();
}

std::vector<Row>& Config::getRows()
{
	return rows;
}

Row& Config::getRowAt(int index)
{
	return rows.at(index);
}

void Config::addRow(int size, std::string chars)
{
	Row row;
	row.setSize(size);
	row.setChars(chars);
	rows.push_back(row);
}