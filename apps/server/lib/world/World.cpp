#include "World.hpp"
#include "WorldConstants.hpp"

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
	std::cout << "Player " << playerID << " is moving from zone " << currentZoneID << " to zone " << destinationZoneID << std::endl;
	Zone::broadcastMessage(destinationZoneID, DatabaseTool::getCharNameFromID(playerID) + " entered the zone.\n");
	DatabaseTool::putCharInZone(playerID, destinationZoneID);
	Zone::broadcastMessage(currentZoneID, DatabaseTool::getCharNameFromID(playerID) + " left the zone.\n");
	return playerLook(playerID, "");
}

string World::playerLook(int playerID, string keyword) {
	int currentZoneID = DatabaseTool::getCharsLocation(playerID);
	boost::trim(keyword);
	return Zone::getDescription(currentZoneID, keyword);
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

// --------Public functions--------

string World::executeCommand(int playerID, Command givenCommand) {
	string command = givenCommand.type;
	string arguments = givenCommand.data;
	cout << command << " " << playerID << " " << arguments << endl;
	if (command == "move") {
		return movePlayer(playerID, arguments);		
	}
	else if (command == "look") {
		return playerLook(playerID, arguments);
	}
	else if (command == "pickup") {
		return playerPickupItem(playerID, arguments);
	}
	return "The command " + command + " was not recognized. Check help for a list of valid commands.\n";
}