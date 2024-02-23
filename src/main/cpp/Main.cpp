#include "editor/Editor.hpp"

int main(int argc, char *argv[])
{
	Editor editor;

	if (argc >= 2) {
		editor.open(argv[1]);
	}

	while (1) {
		editor.refreshScreen();
		editor.processKeypress();
	}
}