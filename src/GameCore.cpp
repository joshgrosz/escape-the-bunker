#include "GameCore.h"
#include "CommandParser.h"
#include <stdio.h>

std::string cmdVerb;
int sleepCount = 0;

RoomData CreateRoom(const std::string& inName, const std::string& inDescription, bool hasKey, bool canLeave) 
{
	RoomData room = {};
	room.Name = inName;
	room.Description = inDescription;
	room.HasKey = hasKey;
	room.CanLeave = canLeave;
	return room;
}

void addRoomExit(RoomData& roomToEdit, const std::string& exitName, int targetRoomIndex, bool isLocked)
{
	RoomExitData exit = {};
	exit.Name = exitName;
	exit.TargetRoomIndex = targetRoomIndex;
	exit.Locked = isLocked;
	roomToEdit.Exits.push_back(exit);
}

void InitializeGame(PlayerState& playerState, WorldState& worldState) 
{
	printf("\t\t\t\t\t\t||\tESCAPE THE BUNKER\t||\n");
	printf("\t\t\t\t\t\t==================================\n");
	printf("Welcome, player!\nIn a land hit by a deadly plague, you are the only remaining person after staying in a bunker you bought.\nNow, it is time to make your way back up to the surface because there is no food left!\n\n");
	
	playerState.currRoomIndex = 0;
	playerState.WantsToLook = true;
	playerState.HasKey = false;

	RoomData room1 = CreateRoom("bedroom", "Your headquarters and office. Its vibrant decorations contrast the bleakness of the pale bunker walls.", true, false);
	addRoomExit(room1, "storage", 1, true);
	addRoomExit(room1, "locked", 2, true);
	worldState.Rooms.push_back(room1);
	
	RoomData room2 = CreateRoom("storage", "Where emptied rations and rusted weaponry are stored.\nBack when the Internet was a thing, this would've been a cool place to run web hosting servers.", true, false);
	addRoomExit(room2, "locked", 3, true);
	addRoomExit(room2, "stairs", 4, false);
	worldState.Rooms.push_back(room2);
	
	RoomData room3 = CreateRoom("locked", "You haven't been past here in so long you barely remember what's behind the door...", true, false);
	addRoomExit(room3, "stairs", 4, false);
	addRoomExit(room3, "generator", 5, false);
	worldState.Rooms.push_back(room3);
	
	RoomData room4 = CreateRoom("locked", "A small locked door lies at the end of the hallway. What could lay inside?", false, false);
	addRoomExit(room4, "bedroom", 0, false);
	worldState.Rooms.push_back(room4);
	
	RoomData room5 = CreateRoom("stairs", "From afar you can see a wide pair of dusty concrete stairs going up.\nWill these carry you to freedom and some food? Try to \"leave\".", false, true);
	addRoomExit(room4, "storage", 1, false);
	addRoomExit(room4, "locked", 2, false);
	worldState.Rooms.push_back(room5);
	
	RoomData room6 = CreateRoom("generator", "Since you're not an electrician you can't remember the last time you even entered this room.", false, false);
	addRoomExit(room6, "locked", 2, false);
	worldState.Rooms.push_back(room6);
}

void GetInput(PlayerState& playerState, const WorldState& worldState) 
{
	playerState.WantsToLook = false;
	playerState.WantsToLeave = false;
	playerState.desiredExit = "";
	playerState.desiredPickup = "";

	printf("What do you do?\n");
	printf("> ");
	TextAdventureCommand command = ParseAdventureCommand();
	cmdVerb = command.Verb;

	if (command.Verb == "quit") 
	{
		playerState.WantsToExit = true;
	}
	else if (command.Verb == "help") 
	{
		printf("Command List: look, quit, go [place], get key, sleep, leave\n");
	}
	else if (command.Verb == "look") 
	{
		playerState.WantsToLook = true;
	}
	else if (command.Verb == "go") 
	{
		playerState.desiredExit = command.Parameter;
	}
	else if (command.Verb == "get") 
	{
		playerState.desiredPickup = command.Parameter;
	}
	else if (command.Verb == "leave") 
	{
		playerState.WantsToLeave = true;
	}
	else if (command.Verb == "sleep") 
	{
		printf("Sleeping...\n");
		if (sleepCount % 2 == 0) 
		{
			printf("It is currently morning according to your watch.\n");
		}
		else printf("It is currently night according to your watch.\n");
		++sleepCount;
		if (sleepCount == 5) 
		{
			printf("You peacefully pass away of starvation... hopefully the afterlife has good Mexican food...");
			playerState.WantsToExit = true;
		}
	}
	else printf("I don't understand\n");
	printf("\n");
}

void RenderGame(const PlayerState& playerState, const WorldState& worldState) 
{
	if (playerState.WantsToLook) 
	{
		RoomData currRoom = worldState.Rooms[playerState.currRoomIndex];
		printf("====================================\n");
		printf("LOCATION: %s\n", currRoom.Name.c_str());
		printf("%s\n\n", currRoom.Description.c_str());

		if (currRoom.HasKey) 
		{
			printf("There is a key in this room...\n\n");
		}
		printf("EXITS:\n");

		for (unsigned int i = 0; i < currRoom.Exits.size(); ++i) 
		{
			printf("%s\n", currRoom.Exits[i].Name.c_str());
		}

		if (playerState.HasKey) 
		{
			printf("INVENTORY:\n");
			printf("Key\n");
		}

		printf("\n");
	}
}

void UpdateGame(PlayerState& playerState, WorldState& worldState) 
{
	RoomData& currRoom = worldState.Rooms[playerState.currRoomIndex];

	if (playerState.desiredExit != "") //if !empty
	{ 
		bool foundExit = false;
		for (unsigned int i = 0; i < currRoom.Exits.size(); ++i) 
		{
			if (playerState.desiredExit == currRoom.Exits[i].Name) 
			{
				foundExit = true;
				if (!currRoom.Exits[i].Locked) 
				{
					playerState.currRoomIndex = currRoom.Exits[i].TargetRoomIndex;
					playerState.WantsToLook = true;

				}
				else {
					if (playerState.HasKey) 
					{
						playerState.currRoomIndex = currRoom.Exits[i].TargetRoomIndex;
						playerState.WantsToLook = true;
						printf("Room unlocked!\n");
					}
					else printf("This door is locked. Search for a key... \n");
				}
			}
		}

		if (!foundExit) 
		{
			printf("I could not find an exit called '%s'\n", playerState.desiredExit.c_str());
		}
	}

	if (playerState.desiredExit == "" && cmdVerb == "go") 
	{
		printf("Please enter an argument.\n");
	}
	else if (playerState.desiredPickup != "") 
	{
		if (playerState.desiredPickup == "key") 
		{
			if (currRoom.HasKey) 
			{
				printf("Found a key in the room!\n");
				playerState.HasKey = true;
				currRoom.HasKey = false;
			}
			else printf("There isn't a key in the room!\n");
		}
		else printf("I don't know what a '%s' is...", playerState.desiredPickup.c_str());
	}
	else if (playerState.WantsToLeave) 
	{
		if (currRoom.CanLeave) 
		{
			printf("You have reached the outside world and left the bunker! Now, what adventures await you?\n");
			playerState.WantsToExit = true;
		}
		else printf("You're still stuck in the bunker, but you can sleep if you want to.\n");
	}
}

void CleanupGame(PlayerState& playerState, WorldState& worldState) 
{
	printf("Quitting...\n\n");
}
