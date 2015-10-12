#include "Zone.hpp"


// --------Private functions--------

static vector<int> getNeighbourZones(int zoneID) {
	// Query database for this zone's neighbours
}

static vector<int> getPlayerIDs(int zoneID) {
	return getAllCharsInZone(zoneID);
}

static vector<int> getNpcIDs(int zoneID) {
	return getAllNpcsInZone(zoneID);
}

static vector<int> getObjectIDs(int zoneID) {
	// Query database for all items who's currentZone is this zone's zoneID
}

// --------Public functions--------

static void setName(int zoneID, string givenName) {
	// Change this zone's name in database
}

static string getName(int zoneID) {
	return getZoneName(zoneID);
}

static void setDescription(int zoneID, string givenDescription) {
	// Change this zone's description in database
}

static string getDescription(int zoneID, string keyword) {
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

static void setNeighbourZone(int zoneID, string direction, int neighbourID) {
	// Change this zone's neighbouring zone (for direction) to new neighbourID
}

static int getNeighbourZone(int zoneID, string direction) {
	return DatabaseTool::getDirectionID(zoneID, direction);
}

static bool roomForMorePlayers(int zoneID) {
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