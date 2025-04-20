#include "editor/Preferences.hpp"
#include <nlohmann/json.hpp>
#include <iostream>
#include <fstream>

Preferences loadPreferences(const std::string& filename)
{
	std::ifstream file(filename);
	nlohmann::json j;
	file >> j;

	Preferences pref;
	pref.word_wrap = j["word_wrap"];

	return pref;
}
