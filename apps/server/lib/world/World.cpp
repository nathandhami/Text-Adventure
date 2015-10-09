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
	//currentZone.playerEnteringZone(playerInfo.name);
	querySetPlayerZone(playerID, destinationZoneID);   //TBD
	return true;
}

string playerLook(int playerID, string keyword) {
	object playerInfo = query(playerID);    // TBD
	int currentZoneID = playerInfo.zoneID;  // TBD
	currentZone.setID(currentZoneID);
	return currentZone.getDescription(keyword);
}

// --------Public functions--------

string executeCommand(Command givenCommand) {
	if (givenCommand.type == "move") {       //TBD
		bool success = movePlayer(givenCommand.playerID, givenCommand.data);  //TBD
		if (!success) {
			return "Unable to move player " + givenCommand.data;  //TBD
		}
		return playerLook(givenCommand.playerID, "");  //TBD
	}
	else if (givenCommand.type == "look") {    //TBD
		return playerLook(givenCommand.playerID, givenCommand.data);  //TBD
	}
	return "The command " + givenCommand + " was not recognized. Check help for a list of valid commands.";
}

World() {
	currentZone = new Zone(0);
}

~World() {
	free(currentZone);
}