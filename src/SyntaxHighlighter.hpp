#ifndef SYNTAX_HIGHLIGHTER_HPP
#define SYNTAX_HIGHLIGHTER_HPP

#include "Row.hpp"
#include <regex>
#include <string>
#include <vector>

enum HighlightType
{
    HL_NORMAL = 0,
    HL_COMMENT,
    HL_KEYWORD,
    HL_TYPE,
    HL_LITERAL,
    HL_STRING,
    HL_NUMBER
};

class SyntaxHighlighter
{
  public:
    SyntaxHighlighter(const std::string& languageFilePath);

    void highlight(Row& row);

  private:
    std::vector<std::string> keywords;
    std::vector<std::string> types;
    std::vector<std::string> literals;
    std::vector<char> strings;
    std::regex numberRegex;
    std::string lineComment;
    std::string blockCommentStart;
    std::string blockCommentEnd;
};

#endif
