#ifndef TERMINAL_H
#define TERMINAL_H

#include "Exceptions.hpp"
#include <termios.h>

class Terminal
{
  public:
    termios originalTerminal;
    Exceptions exception;

    Terminal();
    ~Terminal();

    void disableRawMode();
    void enableRawMode();
};

#endif
