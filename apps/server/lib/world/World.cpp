#include "World.hpp"


// --------Private functions--------

bool movePlayer(int playerID, Direction destination) {
	object playerInfo = query(playerID);    // TBD
	int currentZoneID = playerInfo.zoneID;  // TBD
	currentZone.setID(currentZoneID);
	int destinationZoneID = currentZone.getNeighbourZone(destination);
	currentZone.setID(destinationZoneID);
	if (!currentZone.roomForMorePlayers()) {
		return false;
	}
	currentZone.playerEnteringZone(playerInfo.name);
	querySetPlayerZone(playerID, destinationZoneID);   //TBD
	return true;
}

bool playerLook(int playerID, string keyword) {
	object playerInfo = query(playerID);    // TBD
	int currentZoneID = playerInfo.zoneID;  // TBD
	currentZone.setID(currentZoneID);
	currentZone.getDescription(keyword);
	return true;
}

// --------Public functions--------

bool executeCommand(struct givenCommand) {
	if (givenCommand.command == "move") {       //TBD
		bool success = movePlayer(givenCommand.playerID, givenCommand.argument);  //TBD
		if (!success) {
			return false;    // I don't know if we want to be returning an error message instead of bools
		}
	} 
	else if (givenCommand.command == "look") {    //TBD

	}
	else {
		return false;
	}
	return true
}

World() {
	currentZone = new Zone(0);
}

~World() {
	free(currentZone);
}