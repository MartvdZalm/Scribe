#ifndef CONFIG_H
#define CONFIG_H

#include <vector>
#include <fstream>
#include <filesystem>
#include <cstring>
#include <termios.h>
#include <time.h>
#include <Row.h>

class Config
{
private:
    int coordinateX;
    int coordinateY;
    int rowOff;
    int colOff;
    int screenRows;
    int screenCols;
    int dirty;
    std::vector<Row> rows;
    std::string filename;
    std::string statusMessage;
    time_t statusMessageTime;
    termios origTermios;

public:
    Config();
    
    ~Config();

    termios& getOrigTermios();

    int& getScreenRows();

    int& getScreenCols();

    void setScreenRows(int rows);

    int& getCoordinateX();

    void setCoordinateX(int x);

    void incCoordinateX();

    void decCoordinateX();

    int& getCoordinateY();

    void setCoordinateY(int y);

    void incCoordinateY();

    void decCoordinateY();

    int& getRowOff();

    void setRowOff(int off);

    int& getColOff();

    void setColOff(int off);

    int& getDirty();

    void setDirty(int dirty);

    void incDirty();

    int getNumRows();

    std::vector<Row>& getRows();

    Row& getRowAt(int index);

    void addRow(int index, std::string str);

    std::string getFilename();

    void setFilename(std::string filename);

    std::string getStatusMessage();

    void setStatusMessage(std::string message);

    time_t getStatusMessageTime();

    void saveRows();

    void deleteRow(int index);
};

#endif
