#include "Highlighting.hpp"
#include "Logger.hpp"
#include "UserConfig.hpp"
#include <fstream>
#include <nlohmann/json.hpp>
#include <stdexcept>
#include <unistd.h>

HighlightTheme loadTheme(const std::string& filename)
{
    auto fullPath = UserConfig::get().getPath(filename).string();
    LOG_DEBUG("Loading theme from: " + fullPath);
    std::ifstream file(fullPath);

    if (!file.is_open())
    {
        throw std::runtime_error("Could not open theme file: " + filename);
    }

    nlohmann::json j;

    try
    {
        file >> j;
    }
    catch (const std::exception& e)
    {
        LOG_ERROR(std::string("JSON parse error: ") + e.what());
        throw;
    }

    HighlightTheme theme;

    try
    {
        theme.comment = j.at("comment");
        theme.string = j.at("string");
        theme.number = j.at("number");
        theme.keyword = j.at("keyword");
        theme.type = j.at("type");
        theme.literal = j.at("literal");
        theme.reset = j.at("reset");
    }
    catch (const std::exception& e)
    {
        LOG_ERROR(std::string("Missing JSON key: ") + e.what());
        throw;
    }

    return theme;
}
