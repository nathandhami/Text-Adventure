#include "Zone.hpp"


// --------Private functions--------

vector<int> Zone::getNeighbourZones(int zoneID) {
	// Query database for this zone's neighbours
}

vector<int> Zone::getPlayerIDs(int zoneID) {
	return DatabaseTool::getAllOnlineCharsInZone(zoneID);
}

vector<int> Zone::getNpcIDs(int zoneID) {
	return DatabaseTool::getAllAliveNpcsInZone(zoneID);
}

vector<int> Zone::getObjectIDs(int zoneID) {
	// Query database for all items who's currentZone is this zone's zoneID
}

// --------Public functions--------

void Zone::setName(int zoneID, string givenName) {
	// Change this zone's name in database
}

string Zone::getName(int zoneID) {
	return DatabaseTool::getZoneName(zoneID);
}

void Zone::setDescription(int zoneID, string givenDescription) {
	// Change this zone's description in database
}

void Zone::setNeighbourZone(int zoneID, string direction, int neighbourID) {
	// Change this zone's neighbouring zone (for direction) to new neighbourID
}

int Zone::getNeighbourZone(int zoneID, string direction) {
	return DatabaseTool::getDirectionID(zoneID, direction);
}

bool Zone::roomForMorePlayers(int zoneID) {
	//if ((DatabaseTool::getAllCharsInZone(zoneID).size() >= MAX_PLAYERS) {
	//	return false;
	//}
	return true;
}

void Zone::broadcastMessage(int zoneID, string message) {
	vector<int> playersInZone = DatabaseTool::getAllOnlineCharsInZone(zoneID);
	for (int i = 0; i < playersInZone.size(); i++) {
		Server::sendMessageToCharacter(playersInZone[i], GameCode::NONE, message);
	}
}
