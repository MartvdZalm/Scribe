#include "editor/Editor.hpp"
#include "editor/Preferences.hpp"

int main(int argc, char* argv[])
{
	Preferences pref = loadPreferences("../resources/preferences.json");
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
