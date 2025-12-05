#ifndef ROW_H
#define ROW_H

#include <string>
#include <vector>

class Row
{
  public:
    Row();
    ~Row() = default;

    int getSize() const;
    std::string& getString();
    const std::string& getString() const;

    void setString(const std::string& s);
    void insertChar(int i, char c);
    void insertString(int i, const std::string& input);

    std::string str;
    std::vector<unsigned char> hl;
};

#endif
