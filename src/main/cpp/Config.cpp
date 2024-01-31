#include "Config.h"

Config::Config() : 
coordinateX(0), 
coordinateY(0),
rowOff(0),
colOff(0),
screenRows(0),
screenCols(0),
dirty(0),
rows(),
filename("[No Name]"),
statusMessage("\0"),
statusMessageTime(0),
origTermios() {}

Config::~Config() {}

termios& Config::getOrigTermios()
{
	return this->origTermios;
}

int& Config::getScreenRows()
{
	return this->screenRows;
}

int& Config::getScreenCols()
{
	return this->screenCols;
}

void Config::setScreenRows(int rows)
{
	this->screenRows = rows;
}

int& Config::getCoordinateX()
{
	return this->coordinateX;
}

void Config::setCoordinateX(int x)
{
	this->coordinateX = x;
}

void Config::incCoordinateX()
{
	this->coordinateX++;
}

void Config::decCoordinateX()
{
	this->coordinateX--;
}

int& Config::getCoordinateY()
{
	return this->coordinateY;
}

void Config::setCoordinateY(int y)
{
	this->coordinateY = y;
}

void Config::incCoordinateY()
{
	this->coordinateY++;
}

void Config::decCoordinateY()
{
	this->coordinateY--;
}

int& Config::getRowOff()
{
	return this->rowOff;
}

void Config::setRowOff(int off)
{
	this->rowOff = off;
}

int& Config::getColOff()
{
	return this->colOff;
}

void Config::setColOff(int off)
{
	this->colOff = off;
}

int& Config::getDirty()
{
	return this->dirty;
}

void Config::setDirty(int dirty)
{
	this->dirty = dirty;
}

void Config::incDirty()
{
	this->dirty++;
}

int Config::getNumRows()
{
	return this->rows.size();
}

std::vector<Row>& Config::getRows()
{
	return this->rows;
}

Row& Config::getRowAt(int index)
{
	return this->rows.at(index);
}

void Config::addRow(int index, std::string str)
{
	if (index < 0 || index > getNumRows()) {
		return;
	}

	Row row;
	row.setString(str);

	if (index == getNumRows()) {
		this->rows.push_back(row);
	} else {
		this->rows.insert(this->rows.begin() + index, row);
	}
}

std::string Config::getFilename()
{
	return this->filename;
}

void Config::setFilename(std::string filename)
{
	this->filename = filename;
}

std::string Config:: getStatusMessage()
{
	return this->statusMessage;
}

void Config::setStatusMessage(std::string message)
{
	this->statusMessageTime = time(NULL);
	this->statusMessage = message;
}

time_t Config::getStatusMessageTime()
{
	return this->statusMessageTime;
}

void Config::saveRows()
{
    if (filename == "[No Name]") return;

    std::filesystem::path cwd = std::filesystem::current_path() / filename;
    std::ofstream outputFile(cwd.string(), std::ios::binary);

    if (!outputFile.is_open()) {
        this->setStatusMessage("Can't save! I/O error: " + std::string(std::strerror(errno)));
        return;
    }

    for (Row& row : this->rows) {
        outputFile << row.getString() << '\n';
    }

    outputFile.close();

    std::uintmax_t fileSize = std::filesystem::file_size(cwd);

    setStatusMessage(std::to_string(fileSize) + " bytes written to disk");
}

void Config::deleteRow(int index)
{
	Row *currentRow = &getRowAt(getCoordinateY());
	if (index < 0 || index >= getNumRows()) {
		return;
	}

	if (currentRow->getSize() != 0) {
		Row *row = &getRowAt(getCoordinateY() - 1);
		row->insertString(row->getSize(), currentRow->getString());
	}

	rows.erase(rows.begin() + index);
	incDirty();
}