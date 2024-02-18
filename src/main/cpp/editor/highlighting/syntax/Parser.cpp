#include "editor/highlighting/syntax/Parser.hpp"

Parser::Parser(std::string filename) :
exception()
{
	content("syntaxes/" + filename + ".json");

	if (!content) {
		exception.die("Couldn't open syntax file " + filename + ".json");
	}
}

Parser::~Parser() {}

Parser::parse()
{
	std::string line;
	while (content >> std::noskipws >> c) {

		switch (c) {
			case '{': {

			} break; 

		}
	}
}