#ifndef CONFIG_H
#define CONFIG_H

#include <algorithm>
#include <cctype>
#include <ctype.h>
#include <errno.h>
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <termios.h>
#include <time.h>
#include <unistd.h>

class Config
{
  public:
    int x;
    int y;
    int rowOff;
    int colOff;
    int screenRows;
    int screenCols;

    Config();
    ~Config();
    int getCursorPosition();
    int getWindowSize();
};

#endif