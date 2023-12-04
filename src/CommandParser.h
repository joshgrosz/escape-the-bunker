#pragma once
#include <string>

struct TextAdventureCommand
{
	std::string Verb;
	std::string Parameter;
};

TextAdventureCommand ParseAdventureCommand();
