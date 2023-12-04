#pragma once
#include <vector>
#include <string>

struct PlayerState
{
	int currRoomIndex;
	std::string desiredExit;
	std::string desiredPickup;

	bool WantsToLeave;
	bool WantsToExit;
	bool WantsToLook;
	bool HasKey;
};

struct WorldState
{
	std::vector<struct RoomData> Rooms;
};

struct RoomExitData
{
	std::string Name;
	int TargetRoomIndex;
	bool Locked;
};

struct RoomData 
{
	std::string Name;
	std::string Description;
	std::vector<RoomExitData> Exits;
	bool HasKey;
	bool CanLeave;
};

void InitializeGame(PlayerState& playerState, WorldState& worldState);
void GetInput(PlayerState& playerState, const WorldState& worldState);
void RenderGame(const PlayerState& playerState, const WorldState& worldState);
void UpdateGame(PlayerState& playerState, WorldState& worldState);
void CleanupGame(PlayerState& playerState, WorldState& worldState);
