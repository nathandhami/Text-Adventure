#include "Zone.hpp"


// --------Private functions--------

vector<int> getNeighbourZones(int zoneID) {
	// Query database for this zone's neighbours
}

vector<int> getPlayerIDs(int zoneID) {
	return getAllCharsInZone(zoneID);
}

vector<int> getNpcIDs(int zoneID) {
	return getAllNpcsInZone(zoneID);
}

vector<int> getObjectIDs(int zoneID) {
	// Query database for all items who's currentZone is this zone's zoneID
}

// --------Public functions--------

void setName(int zoneID, string givenName) {
	// Change this zone's name in database
}

string getName(int zoneID) {
	return getZoneName(zoneID);
}

void setDescription(int zoneID, string givenDescription) {
	// Change this zone's description in database
}

string getDescription(int zoneID, string keyword) {
	string description = "";
	if (keyword == "") {
		description = getZoneDesc(zoneID);
		if (description == "") {
			return "You seem to be floating in a void. You question your own existence.\n";
		}
		return description;
	}
	else if (isDirection(keyword)) {
		description = DatabaseTool::getDirectionDesc(zoneID, keyword);
		if (description == "") {
			return "There is nothing in that direction\n";
		}
	}
	else {
		description = DatabaseTool::getZoneExtendedDescription(zoneID, keyword) {
			if (description == "") {
				return "You can't describe the " + keyword + "\n";
			}
		}
	}
	return description;
}

void setNeighbourZone(int zoneID, string direction, int neighbourID) {
	// Change this zone's neighbouring zone (for direction) to new neighbourID
}

int getNeighbourZone(int zoneID, string direction) {
	return DatabaseTool::getDirectionID(zoneID, direction);
}

bool roomForMorePlayers(int zoneID) {
	if ((DatabaseTool::getAllCharsInZone(zoneID).size() >= MAX_PLAYERS) {
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