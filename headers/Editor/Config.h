#ifndef CONFIG_H
#define CONFIG_H

#include <vector>
#include <termios.h>
#include <Editor/Row.h>

class Config
{
private:
    int coordinateX;
    int coordinateY;
    int screenRows;
    int screenCols;
    std::vector<Row> rows;
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

    int getNumRows();

    std::vector<Row>& getRows();

    Row& getRowAt(int index);

    void addRow(int size, std::string chars);
};

#endif
