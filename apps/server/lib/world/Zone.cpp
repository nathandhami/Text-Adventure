#include "Zone.hpp"


// --------Private functions--------

vector<int> Zone::getPlayerIDs(int zoneID) {
	return DatabaseTool::getAllOnlineCharsInZone(zoneID);
}

vector<int> Zone::getNpcIDs(int zoneID) {
	return DatabaseTool::getAllAliveNpcsInZone(zoneID);
}

// --------Public functions--------

string Zone::getName(int zoneID) {
	return DatabaseTool::getZoneName(zoneID);
}

int Zone::getNeighbourZone(int zoneID, string direction) {
	return DatabaseTool::getDirectionID(zoneID, direction);
}

void Zone::broadcastMessage(int zoneID, string message, vector<int> exceptPlayers) {
	vector<int> playersInZone = DatabaseTool::getAllOnlineCharsInZone(zoneID);
	for (int playersInZoneIndex = 0; playersInZoneIndex < playersInZone.size(); playersInZoneIndex++) {
		bool transmitMessage = true;
		for (int exceptionsIndex = 0; exceptionsIndex < exceptPlayers.size(); exceptionsIndex++) {
			//cout << playersInZone[playersInZoneIndex] << "  WHY NOT EQUAL  " << exceptPlayers[exceptionsIndex] << endl;
			// For some weird reason I'm getting 1 == 0 from this if, but when calling sendMessage it gives the correct playerID
			if (playersInZone[playersInZoneIndex] == exceptPlayers[exceptionsIndex]) {
				transmitMessage = false;
			}
		}
		if (transmitMessage) {
			Server::sendMessageToCharacter(playersInZone[playersInZoneIndex], GameCode::STATUS, message);
		}
	}
}

void Zone::broadcastMessage(int zoneID, string message) {
	Zone::broadcastMessage(zoneID, message, vector<int>());
}
