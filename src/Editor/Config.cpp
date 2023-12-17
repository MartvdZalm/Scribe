#include "Editor/Config.h"

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

void Config::setScreenRows(int rows)
{
	this->screenRows = rows;
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

int& Config::getDirty()
{
	return dirty;
}

void Config::setDirty(int dirty)
{
	this->dirty = dirty;
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

void Config::addRow(std::string str)
{
	Row row;
	row.setString(str);
	rows.push_back(row);
}

std::string Config::getFilename()
{
	return filename;
}

void Config::setFilename(std::string filename)
{
	this->filename = filename;
}

std::string Config:: getStatusMessage()
{
	return statusMessage;
}

void Config::setStatusMessage(std::string message)
{
	statusMessageTime = time(NULL);
	statusMessage = message;
}

time_t Config::getStatusMessageTime()
{
	return statusMessageTime;
}

void Config::saveRows()
{
    if (filename == "[No Name]") return;

    std::filesystem::path cwd = std::filesystem::current_path() / filename;
    std::ofstream outputFile(cwd.string(), std::ios::binary);

    if (!outputFile.is_open()) {
        setStatusMessage("Can't save! I/O error: " + std::string(std::strerror(errno)));
        return;
    }

    for (Row& row : rows) {
        outputFile << row.getString() << '\n';
    }

    outputFile.close();

    std::uintmax_t fileSize = std::filesystem::file_size(cwd);

    setStatusMessage(std::to_string(fileSize) + " bytes written to disk");
}