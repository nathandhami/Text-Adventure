#include "World.hpp"


// --------Private functions--------

bool movePlayer(int playerID, Destination destination) {
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

// --------Public functions--------

World() {
	currentZone = new Zone(0);
}

~World() {

}