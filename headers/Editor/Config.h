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
    int rowOff;
    int colOff;
    int screenRows;
    int screenCols;
    std::vector<Row> rows;
    std::string filename;
    termios origTermios;

public:
    Config();
    
    ~Config();

    termios& getOrigTermios();

    int& getScreenRows();

    int& getScreenCols();

    void setScreenRows(int rows);

    int& getCoordinateX();

    int& getCoordinateY();

    void setCoordinateX(int x);

    void setCoordinateY(int y);

    int& getRowOff();

    void setRowOff(int off);

    int& getColOff();

    void setColOff(int off);

    int getNumRows();

    std::vector<Row>& getRows();

    Row& getRowAt(int index);

    void addRow(std::string str);

    std::string getFilename();

    void setFilename(std::string filename);
};

#endif
