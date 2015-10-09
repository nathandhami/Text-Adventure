#include "World.hpp"
#include <boost/algorithm/string.hpp>


// --------Private functions--------

bool movePlayer(int playerID, string destination) {
	boost::to_upper(destination);
	if (!isDirection(destination)) {
		return false;
	}
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
	boost::to_upper(keyword);
	object playerInfo = query(playerID);    // TBD
	int currentZoneID = playerInfo.zoneID;  // TBD
	currentZone.setID(currentZoneID);
	return currentZone.getDescription(keyword);
}

// --------Public functions--------

string executeCommand(Command givenCommand) {
	int playerID = givenCommand.playerID;
	string command = givenCommand.type;
	string arguments = givenCommand.data;

	if (command == "move") {       //TBD
		bool success = movePlayer(playerID, arguments);  //TBD
		if (!success) {
			return "Unable to move " + arguments + "\n";  //TBD
		}
		return playerLook(playerID, "");  //TBD
	}
	else if (givenCommand.type == "look") {    //TBD
		return playerLook(playerID, arguments);  //TBD
	}
	return "The command " + command + " was not recognized. Check help for a list of valid commands.\n";
}

World() {
	currentZone = new Zone(0);
}

~World() {
	free(currentZone);
}