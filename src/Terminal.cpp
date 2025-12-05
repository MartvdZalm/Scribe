#include "Terminal.hpp"

Terminal::Terminal() : originalTerminal(), exception()
{
    if (tcgetattr(STDIN_FILENO, &this->originalTerminal) == -1)
    {
        this->exception.die("tcgetattr");
    }
}

Terminal::~Terminal() {}

void Terminal::enableRawMode()
{
    if (tcgetattr(STDIN_FILENO, &this->originalTerminal) == -1)
    {
        this->exception.die("tcgetattr");
    }

    termios raw = this->originalTerminal;
    raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
    raw.c_oflag &= ~(OPOST);
    raw.c_cflag |= ~(CS8);
    raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
    raw.c_cc[VMIN] = 0;
    raw.c_cc[VTIME] = 1;

    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1)
    {
        this->exception.die("tcsetattr");
    }
}

void Terminal::disableRawMode()
{
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &originalTerminal);
}
