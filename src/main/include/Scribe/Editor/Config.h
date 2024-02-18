#ifndef CONFIG_H
#define CONFIG_H

#include <unistd.h>
#include <termios.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <cctype>
#include <time.h>

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