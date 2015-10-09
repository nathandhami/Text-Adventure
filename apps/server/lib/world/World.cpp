#include "World.hpp"


// --------Private functions--------

bool movePlayer(int playerID, string destination) {
	boost::to_upper(destination);
	if (!isDirection(destination)) {
		return false;
	}
	int currentZoneID = getCharsLocation(playerID);
	currentZone.setID(currentZoneID);
	int destinationZoneID = currentZone.getNeighbourZone(destination);
	currentZone.setID(destinationZoneID);
	if (!currentZone.roomForMorePlayers()) {
		return false;
	}
	//currentZone.playerEnteringZone(playerInfo.name);
	putCharInZone(playerID, destinationZoneID);
	return true;
}

string playerLook(int playerID, string keyword) {
	boost::to_upper(keyword);
	int currentZoneID = getCharsLocation(playerID);
	currentZone.setID(currentZoneID);
	return currentZone.getDescription(keyword);
}

// --------Public functions--------

string executeCommand(Command givenCommand) {
	int playerID = givenCommand.playerID;
	string command = givenCommand.type;
	string arguments = givenCommand.data;

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

World() {
	currentZone = new Zone(0);
}

~World() {
	free(currentZone);
}