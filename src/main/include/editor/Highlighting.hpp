#ifndef HIGHLIGHTING_HPP
#define HIGHLIGHTING_HPP

#include <string>

struct HighlightTheme {
    std::string comment;
    std::string string;
    std::string number;
    std::string keyword;
    std::string type;
    std::string literal;
    std::string reset;
};

inline HighlightTheme defaultTheme = {
    "\x1b[32m",  // comment (green)
    "\x1b[34m",  // string (blue)
    "\x1b[31m",  // number (red)
    "\x1b[36m",  // keyword (cyan)
    "\x1b[33m",  // type (yellow)
    "\x1b[35m",  // literal (magenta)
    "\x1b[0m"    // reset
};

HighlightTheme loadTheme(const std::string& filename);

#endif
