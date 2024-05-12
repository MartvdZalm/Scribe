#include "editor/Editor.hpp"
#include "editor/highlighting/Highlighting.hpp"

int main(int argc, char *argv[])
{
	Editor editor;

	// Highlighting highlighting("resources/syntax/knight.json");

	if (argc >= 2) {
		editor.open(argv[1]);
	}

	while (1) {
		editor.refreshScreen();
		editor.processKeypress();

		// Row& row = editor.rows.at(0);
		// highlighting.checkRow(&row);
	}
}