#ifndef PARSER_HPP
#define PARSER_HPP

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <fstream>
#include <iostream>
#include <vector>

#include "rapidjson/filereadstream.h"
#include "rapidjson/document.h"
#include "rapidjson/error/error.h"

class Parser
{
public:
    Parser(const std::string& filePath);
    ~Parser();
    bool parse();
    const std::unordered_set<std::string>& getKeywords() const { return keywordSet; }
    const std::unordered_set<std::string>& getTypes() const { return typeSet; }
    const std::string& getLineCommentStart() const { return lineCommentStart; }
    const std::string& getBlockCommentStart() const { return blockCommentStart; }
    const std::string& getBlockCommentEnd() const { return blockCommentEnd; }
    const std::vector<std::string>& getStringDelimiters() const { return stringDelimiters; }


// private:
    std::string filePath;
    rapidjson::Document document;
    std::unordered_set<std::string> keywordSet;
    std::unordered_set<std::string> typeSet;
    std::string lineCommentStart;
    std::string blockCommentStart;
    std::string blockCommentEnd;
    std::vector<std::string> stringDelimiters;
};

#endif