#include "World.hpp"

// --------Private variables--------
	
bool World::keepRespawning = false;

std::thread World::respawnThread;

// --------Private functions--------

string World::movePlayer(int playerID, string destination) {
	boost::trim(destination);
	int currentZoneID = DatabaseTool::getCharsLocation(playerID);
	int destinationZoneID = Zone::getNeighbourZone(currentZoneID, destination);
	if ( destinationZoneID == 0 ) {
		return "Unable to move " + destination + "\n";
	}
	if (!Zone::roomForMorePlayers(destinationZoneID)) {
		return "Unable to move " + destination + ", it is full.\n";
	}

	Combat::endCombat(playerID, "");

	std::cout << "Player " << playerID << " is moving from zone " << currentZoneID << " to zone " << destinationZoneID << std::endl;
	Zone::broadcastMessage(destinationZoneID, DatabaseTool::getCharNameFromID(playerID) + " entered the zone.\n");

	DatabaseTool::putCharInZone(playerID, destinationZoneID);
	Zone::broadcastMessage(currentZoneID, DatabaseTool::getCharNameFromID(playerID) + " left the zone.\n");
	return playerLook(playerID, "");
}

string World::playerLook(int playerID, string keyword) {
	boost::trim(keyword);
	return DatabaseTool::look(playerID, keyword);
}

string World::playerLookAt(int playerID, string keyword) {
	boost::trim(keyword);
	return DatabaseTool::look(playerID, keyword);
}

string World::playerPickupItem(int playerID, string item) {
	int currentZoneID = DatabaseTool::getCharsLocation(playerID);
	boost::trim(item);
	if (DatabaseTool::pickUp(playerID, item)) {
		Zone::broadcastMessage(currentZoneID, DatabaseTool::getCharNameFromID(playerID) + " picked up the " + item + "\n");
		return "You pick up the " + item + ".\n";
	}
	return "The " + item + " is not in the room or cannot be picked up.\n";
}

void World::runRespawn() {
	int counter = 0;
	while (keepRespawning) {
		counter++;
		if (counter >= RESPAWN_TIME_SECONDS) {
			DatabaseTool::respawnAll();
			counter = 0;
		}
		sleep(1);
	}
}

// --------Public functions--------

string World::executeCommand(int playerID, Command givenCommand) {
	string command = givenCommand.type;
	string arguments = givenCommand.data;
	cout << command << " " << playerID << " " << arguments << endl;
	if (Combat::isInCombat(playerID)) {
		return "You cannot " + command + " while in combat.\n";
	}
	if (command == "move") {
		return movePlayer(playerID, arguments);
	}
	else if (command == "look") {
		return playerLook(playerID, arguments);
	}
	else if (command == "look at") {
		return playerLook(playerID, arguments);
	}
	else if (command == "pickup") {
		return playerPickupItem(playerID, arguments);
	}
	return "The command " + command + " was not recognized. Check help for a list of valid commands.\n";
}

bool World::isRespawnLoopRunning() {
	return keepRespawning;
}

void World::startRespawnLoop() {
	if (!World::isRespawnLoopRunning()) {
		keepRespawning = true;
		respawnThread = std::thread(&World::runRespawn);
	}
}

void World::stopRespawnLoop() {
	keepRespawning = false;
	respawnThread.join();
}

void World::respawnImmediately() {
	DatabaseTool::respawnAll();
}