#include "editor/Preferences.hpp"
#include <nlohmann/json.hpp>
#include <iostream>
#include <fstream>

Preferences loadPreferences(const std::string& filename)
{
	std::ifstream file(filename);

	if (!file.is_open()) {
		throw std::runtime_error("Couldn't open preferences.json file!");
	}

	nlohmann::json j;
	file >> j;

	Preferences pref;
	pref.word_wrap = j["word_wrap"];

	return pref;
}
