#include "Zone.hpp"


// --------Private functions--------

vector<int> getNeighbourZones() {
	// Query database for this zone's neighbours
}

vector<int> getPlayerIDs() {
	return getAllCharsInZone(zoneID);
}

vector<int> getNpcIDs() {
	return getAllNpcsInZone(zoneID);
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
	return getZoneName(zoneID);
}

void setDescription(string givenDescription) {
	// Change this zone's description in database
}

string getDescription(string keyword) {
	string description = "";
	if (keyword == "") {
		description = getZoneDesc(zoneID);
		if (description == "") {
			return "You seem to be floating in a void. You question your own existence, realize the irony of this, and decide to cry.\n";
		}
		return description;
	}
	else if (isDirection(keyword)) {
		description = getDirectionDesc(zoneID, keyword);
		if (description == "") {
			return "There is nothing in that direction\n";
		}
	}
	else {
		description = getzoneExtendedDescription(zoneID) {
			if (description == "") {
				return "You can't describe the " + keyword + "\n";
			}
		}
	}
	return description;
}

void setNeighbourZone(string direction, int neighbourID) {
	// Change this zone's neighbouring zone (for direction) to new neighbourID
}

int getNeighbourZone(string direction) {
	return getDirectionID(zoneID, direction);
}

bool roomForMorePlayers() {
	if ((getPlayerIDs().size() >= MAX_PLAYERS) {
		return false;
	}
	return true;
}

/*     
void playerEnteringZone(int playerName) {
	vector<int> playersInZone = getPlayerIDs();
	for (int i = 0; i < playersInZone.size(); i++) {
		// Notify playersInZone[i] that playerName entered the zone
	}
}
*/

Zone::Zone(int givenZoneID) {
	zoneID = givenZoneID;
}

Zone::~Zone() {

}