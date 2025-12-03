#include "Editor.hpp"
#include "Preferences.hpp"
#include "Logger.hpp"
#include "UserConfig.hpp"

int main(int argc, char* argv[])
{
    Logger::initialize("Scribe");
    UserConfig::initialize("Scribe");

    Preferences pref = loadPreferences(UserConfig::get().getPath("preferences.json").string());

	Editor editor;
	editor.setPreferences(&pref);

	if (argc >= 2) {
		editor.open(std::string(argv[1]));
	}

	while (1) {
		editor.refreshScreen();
		editor.processKeypress();
	}
}
