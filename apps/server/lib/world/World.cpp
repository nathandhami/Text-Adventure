#include "World.hpp"


// --------Private functions--------

static bool World::movePlayer(int playerID, string destination) {
	boost::to_upper(destination);
	if (!isDirection(destination)) {
		return false;
	}
	int currentZoneID = DatabaseTool::getCharsLocation(playerID);
	int destinationZoneID = Zone::getNeighbourZone(currentZoneID, destination);
	if (!Zone::roomForMorePlayers(destinationZoneID)) {
		return false;
	}
	DatabaseTool::putCharInZone(playerID, destinationZoneID);
	return true;
}

static string World::playerLook(int playerID, string keyword) {
	int currentZoneID = DatabaseTool::getCharsLocation(playerID);
	boost::to_upper(keyword);
	return Zone::getDescription(currentZoneID, keyword);
}

// --------Public functions--------

static string World::executeCommand(int playerID, Command* givenCommand) {
	string command = givenCommand->type;
	string arguments = givenCommand->data;
	cout << 
	if (command == "move") {
		bool success = movePlayer(playerID, arguments);
		if (!success) {
			return "Unable to move " + arguments + "\n";
		}
		return playerLook(playerID, "");
	}
	else if (command == "look") {
		return playerLook(playerID, arguments);
	}
	return "The command " + command + " was not recognized. Check help for a list of valid commands.\n";
}