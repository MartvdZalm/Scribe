#include "Preferences.hpp"
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>


Preferences loadPreferences(const std::string &filename) {
  std::ifstream file(filename);

  if (!file.is_open()) {
    throw std::runtime_error("Couldn't open preferences.json file!");
  }

  nlohmann::json j;
  file >> j;

  Preferences pref;
  pref.word_wrap = j["word_wrap"];

  if (j.contains("theme")) {
    pref.theme = j["theme"];
  } else {
    pref.theme = ""; // Empty means use default theme
  }

  return pref;
}
