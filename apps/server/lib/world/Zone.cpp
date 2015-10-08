#include "Zone.hpp"


// --------Private functions--------

vector<int> getNeighbourZones() {
	// Query database for this zone's neighbours
}

vector<int> getPlayerIDs() {
	// Query database for all players who's currentZone is this zone's zoneID
}

vector<int> getNpcIDs() {
	// Query database for all NPCs who's currentZone is this zone's zoneID
}

vector<int> getObjectIDs() {
	// Query database for all items who's currentZone is this zone's zoneID
}

// --------Public functions--------

void setID(int givenID) {
	zoneID = givenID;
}

int getID() {
	return zoneID;
}

void setName(string givenName) {
	// Change this zone's name in database
}

string getName() {
	// Query database for this zone's name
}

void setDescription(string givenDescription) {
	// Change this zone's description in database
}

string getDescription() {
	// Query database for this zone's description
}

void setNeighbourZone(int direction, int neighbourID) {
	// Change this zone's neighbouring zone (for direction) to new neighbourID
}

int getNeighbourZone(int direction) {
	// Query database for this zone's neighbour (in direction)
}

bool roomForMorePlayers() {
	if ((getPlayerIDs().size() >= MAX_PLAYERS) {
		return false;
	}
	return true;
}

void playerEnteringZone(int playerName) {
	vector<int> playersInZone = getPlayerIDs();
	for (int i = 0; i < playersInZone.size(); i++) {
		// Notify playersInZone[i] that playerName entered the zone
	}
}

Zone::Zone(int givenZoneID) {
	zoneID = givenZoneID;
}

Zone::~Zone() {

}