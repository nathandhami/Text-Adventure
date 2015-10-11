#include "World.hpp"


// --------Private functions--------

bool World::movePlayer(int playerID, string destination) {
	boost::to_upper(destination);
	if (!isDirection(destination)) {
		return false;
	}
	int currentZoneID = DatabaseTool::getCharsLocation(playerID);
	World::currentZone->setID(currentZoneID);
	int destinationZoneID = World::currentZone->getNeighbourZone(destination);
	World::currentZone->setID(destinationZoneID);
	if (!World::currentZone->roomForMorePlayers()) {
		return false;
	}
	//currentZone.playerEnteringZone(playerInfo.name);
	DatabaseTool::putCharInZone(playerID, destinationZoneID);
	return true;
}

string World::playerLook(int playerID, string keyword) {
	boost::to_upper(keyword);
	int currentZoneID = DatabaseTool::getCharsLocation(playerID);
	World::currentZone->setID(currentZoneID);
	return World::currentZone->getDescription(keyword);
}

// --------Public functions--------

string World::executeCommand(Command givenCommand) {
	int playerID = givenCommand->playerID;
	string command = givenCommand->type;
	string arguments = givenCommand->data;

	if (command == "move") {
		bool success = movePlayer(playerID, arguments);
		if (!success) {
			return "Unable to move " + arguments + "\n";
		}
		return playerLook(playerID, "");
	}
	else if (command == "look") {
		return playerLook(playerID, arguments);
	}
	return "The command " + command + " was not recognized. Check help for a list of valid commands.\n";
}

World::World() {
	World::currentZone = new Zone(0);
}

World::~World() {
	delete World::currentZone;
}