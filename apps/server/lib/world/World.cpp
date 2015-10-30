#include "World.hpp"
#include "WorldConstants.hpp"

// --------Private functions--------

bool World::movePlayer(int playerID, string destination) {
	boost::to_upper(destination);
	if (!WorldConstants::isDirection(destination)) {
		return false;
	}
	int currentZoneID = DatabaseTool::getCharsLocation(playerID);
	int destinationZoneID = Zone::getNeighbourZone(currentZoneID, destination);
	if (!Zone::roomForMorePlayers(destinationZoneID)) {
		return false;
	}
	std::cout << "Zone: " << destinationZoneID << std::endl;
	if ( destinationZoneID == 0 ) {
		return false;
	}
	DatabaseTool::putCharInZone(playerID, destinationZoneID);
	return true;
}

string World::playerLook(int playerID, string keyword) {
	int currentZoneID = DatabaseTool::getCharsLocation(playerID);
	boost::to_upper(keyword);
	return Zone::getDescription(currentZoneID, keyword);
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
		if (!movePlayer(playerID, arguments)) {
			return "Unable to move " + arguments + "\n";
		}
		return playerLook(playerID, "");
	}
	else if (command == "look") {
		return playerLook(playerID, arguments);
	}
	return "The command " + command + " was not recognized. Check help for a list of valid commands.\n";
}