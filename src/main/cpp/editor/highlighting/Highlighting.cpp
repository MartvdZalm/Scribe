#include "editor/highlighting/Highlighting.hpp"

Highlighting::Highlighting(const std::string& filePath) :
parser(filePath)
{
	parser.parse();
}

Highlighting::~Highlighting() {}

int Highlighting::checkRow(Row* row)
{
	std::string& input = row->getString();
	int charAdded = 0;

	for (const std::string& keyword : parser.keywordSet) {
		size_t pos = 0;

		while ((pos = input.find(keyword, pos)) != std::string::npos) {

			input.insert(pos, ANSI_COLOR_RED);
			input.insert(pos + keyword.size() + ANSI_COLOR_RED.size(), ANSI_COLOR_RESET);

			pos += keyword.size() + ANSI_COLOR_RED.size() + ANSI_COLOR_RESET.size();

			charAdded += ANSI_COLOR_RED.size() + ANSI_COLOR_RESET.size();
		}
	}

	return charAdded;
}