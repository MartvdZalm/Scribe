#ifndef ROW_H
#define ROW_H

#include <string>

class Row
{
  public:
    std::string str;
    unsigned char* hl;

    Row();

    ~Row();

    int getSize();

    std::string& getString();

    void setString(const std::string& s);

    void insertChar(int i, char c);

    void insertString(int i, std::string input);
};

#endif
