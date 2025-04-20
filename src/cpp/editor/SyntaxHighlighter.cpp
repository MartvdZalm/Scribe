#include "editor/SyntaxHighlighter.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <regex>
#include <nlohmann/json.hpp>

SyntaxHighlighter::SyntaxHighlighter(const std::string& languageFilePath)
{
    std::ifstream file(languageFilePath);
    if (!file)
    {
        throw std::runtime_error("Could not open syntax file: " + languageFilePath);
    }

    nlohmann::json j;
    file >> j;

    keywords = j["keywords"].get<std::vector<std::string>>();
    types = j["types"].get<std::vector<std::string>>();
    literals = j["literals"].get<std::vector<std::string>>();

    for (const auto& s : j["strings"]) {
        if (!s.empty()) {
            strings.push_back(s.get<std::string>()[0]);
        }
    }

    numberRegex = std::regex(j["numbers"]["regex"].get<std::string>());

    lineComment = j["comments"]["line"].get<std::string>();
    blockCommentStart = j["comments"]["block"]["start"].get<std::string>();
    blockCommentEnd = j["comments"]["block"]["end"].get<std::string>();
}

void SyntaxHighlighter::highlight(Row& row)
{
    row.hl = new unsigned char[row.str.size()];
    std::fill(row.hl, row.hl + row.str.size(), HL_NORMAL);

    std::string& text = row.str;

    size_t commentPos = text.find(lineComment);
    if (commentPos != std::string::npos)
    {
        std::fill(row.hl + commentPos, row.hl + text.size(), HL_COMMENT);
        return;
    }

    for (char quote : strings)
    {
        bool inString = false;
        size_t start = 0;
        for (size_t i = 0; i < text.size(); i++)
        {
            if (text[i] == quote)
            {
                if (inString)
                {
                    std::fill(row.hl + start, row.hl + i + 1, HL_STRING);
                    inString = false;
                }
                else
                {
                    start = i;
                    inString = true;
                }
            }
        }
    }

    std::smatch match;
    std::string::const_iterator searchStart(text.cbegin());
    while (std::regex_search(searchStart, text.cend(), match, numberRegex))
    {
        size_t start = match.position();
        size_t length = match.length();
        std::fill(row.hl + start, row.hl + start + length, HL_NUMBER);
        searchStart = match.suffix().first;
    }

    auto highlightWords = [&](const std::vector<std::string>& words, unsigned char color) {
        for (const auto& word : words)
        {
            size_t pos = text.find(word);
            while (pos != std::string::npos)
            {
                if ((pos == 0 || !isalnum(text[pos - 1])) &&
                    (pos + word.length() == text.size() || !isalnum(text[pos + word.length()])))
                {
                    std::fill(row.hl + pos, row.hl + pos + word.length(), color);
                }
                pos = text.find(word, pos + word.length());
            }
        }
    };

    highlightWords(keywords, HL_KEYWORD);
    highlightWords(types, HL_TYPE);
    highlightWords(literals, HL_LITERAL);
}


