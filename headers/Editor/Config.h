#ifndef CONFIG_H
#define CONFIG_H

#include <termios.h>
#include <Editor/Row.h>

class Config
{
private:
    int coordinateX;
    int coordinateY;
    int screenRows;
    int screenCols;
    int numRows;
    Row row;
    termios origTermios;

public:
    Config();
    
    ~Config();

    termios& getOrigTermios();

    int& getScreenRows();

    int& getScreenCols();

    int& getCoordinateX();

    int& getCoordinateY();

    void setCoordinateX(int x);

    void setCoordinateY(int y);

    int& getNumRows();

    void setNumRows(int numRows);

    Row& getRow();
};

#endif
