#include "CommandParser.h"
#include <vector>

TextAdventureCommand ParseAdventureCommand()
{
	TextAdventureCommand retCommand = {};
	
	const unsigned int INPUT_CHAR_LIMIT = 256;
	char inputStr[INPUT_CHAR_LIMIT];

	char* pRet = fgets(inputStr, INPUT_CHAR_LIMIT, stdin);

	for (int i = 0; i < INPUT_CHAR_LIMIT; ++i)
	{
		inputStr[i] = tolower(inputStr[i]);
	}

	std::vector<std::string> tokens;
	char* next_token = nullptr;
	char *token = strtok_s(inputStr, " \t\n", &next_token);
	while (token)
	{
		tokens.push_back(token);
		token = strtok_s(nullptr, " \t\n", &next_token);
	}

	bool bParsed = false;
	if (tokens.size() >= 1)
	{
		retCommand.Verb = tokens[0];

		if (tokens.size() >= 2)
		{
			retCommand.Parameter = tokens[1];
		}
	}

	return retCommand;
}
