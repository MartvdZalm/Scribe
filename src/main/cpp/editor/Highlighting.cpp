#include "Highlighting.hpp"
#include <fstream>
#include <nlohmann/json.hpp>

HighlightTheme loadTheme(const std::string& filename)
{
    std::ifstream file(filename);
    nlohmann::json j;
    file >> j;

    HighlightTheme theme;
    theme.comment = j["comment"];
    theme.string = j["string"];
    theme.number = j["number"];
    theme.keyword = j["keyword"];
    theme.type = j["type"];
    theme.literal = j["literal"];
    theme.reset = j["reset"];

    return theme;
}
