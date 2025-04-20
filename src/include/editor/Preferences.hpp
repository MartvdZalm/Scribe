#ifndef PREFERENCES_HPP
#define PREFERENCES_HPP

#include <string>

struct Preferences
{
	bool word_wrap;
};

Preferences loadPreferences(const std::string& filename);

#endif
