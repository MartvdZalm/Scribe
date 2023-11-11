#include "Editor/Config.h"

Config::Config() : 
coordinateX(0), 
coordinateY(0), 
numRows(0),
origTermios(),
row() {}

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

int& Config::getNumRows()
{
	return numRows;
}

void Config::setNumRows(int numRows)
{
	this->numRows = numRows;
}

Row& Config::getRow()
{
	return row;
}