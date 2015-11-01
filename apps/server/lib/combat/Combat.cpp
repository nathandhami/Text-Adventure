#include "Combat.hpp"


// --------Private functions--------

CombatInstance *Combat::getCombatInstance(int playerID) {
	for (long instanceIndex = 0; instanceIndex < (long)combatInstances.size(); instanceIndex++) {
		if (combatInstances.at(instanceIndex).isCombatant(playerID) {
			return &combatInstance.at(instanceIndex);
		}
	}
	return NULL;
}

// Violates the idea of a function not taking many lines
string Combat::startCombat(int playerID, string arguments) {
	if (Combat::isInCombat(playerID)) {
		return "Finish your current fight before starting another, you barbarian.\n";
	}

	deque<std::string> parsedArgument;
	boost::split(parsedArgument, boost::trim(arguments), boost::is_any_of(" "));
	
	int enemyType = PLAYER_OR_NPC;
	string enemyName = "";
	for (int parsedArgumentIndex = 0; parsedArgumentIndex < parsedArgument; parsedArgumentIndex++) {
		enemyName = parsedArgument.at(parsedArgumentIndex);
		if (enemyName.find_first_not_of(' ') != std::string::npos) {
			if (boost::to_lower(enemyName) == "player") {
				enemyType = PLAYER_ONLY;
			}
			else if (boost::to_lower(enemyName) == "npc") {
				enemyType = NPC_ONLY;
			}
			else {
				break;
			}
		}
	}
	if (enemyName == "") {
		return "No need to take on the whole world, just fight one enemy at a time.\n" + HOW_TO_START_FIGHT;
	}

	// This is pretty ugly, but I can't think of a cleaner way to do it
	int enemyPlayerID = 0;
	if (enemyType == PLAYER_OR_NPC || enemyType == PLAYER) {
		enemyPlayerID = DatabaseTool::getCharIDFromName(enemyName);
	}
	int enemyNpcID = 0;
	if (enemyType == PLAYER_OR_NPC || enemyType == NPC) {
		enemyNpcID = DatabaseTool::getNpcInstanceIDFromName(enemyName, DatabaseTool::getCharsLocation(playerID));
	}

	if (enemyPlayerID > 0 && enemyNpcID > 0) {
		return "Do you want to attack the player named " + enemyName + " or the NPC named " + enemyName "?\nPlease clarify by using one of these commands\n    fight player <name>\n    fight npc <name>\n";
	}
	else if (enemyPlayerID > 0) {
		if (Combat::isInCombat(enemyPlayerID, PLAYER_ONLY)) {
			return enemyName + " is already in combat.\n";
		}
		combatInstances.push_back(CombatInstance(playerID, enemyPlayerID, PLAYER, ));
	}
	else if (enemyNpcID > 0) {
		if (Combat::isInCombat(enemyNpcID, NPC_ONLY)) {
			return enemyName + " is already in combat.\n";
		}
		combatInstances.push_back(CombatInstance(playerID, enemyNpcID, NPC));
	}
	else {
		return "There is no " + enemyName + " in your zone.\n";
	}

	return "";
}

string Combat::queueAttack(int playerID) {
	if (!Combat::isInCombat(playerID)) {
		return "Calm down, you're not even in combat.\n" + HOW_TO_START_FIGHT;
	}
	CombatInstance *instance = getCombatInstance(playerID);
	if (*instance == NULL) {
		cout << "PlayerID " << playerID << "'s CombatInstance does not exist even though they are in combat." << endl;
		DatabaseTool::setCombatFlag(playerID, false, DatabaseTool::Target.character);
		return "Unfortunately, something seems to have failed in our combat system, your fight could not be found.\nYou have been removed from combat.\n";
	}
	instance->combatantAttack(playerID);
	return "";
}

string Combat::retreat(int playerID) {
	if (!Combat::isInCombat(playerID)) {
		return "Calm down, you're not even in combat.\n";
	}
	CombatInstance *instance = getCombatInstance(playerID);
	if (*instance == NULL) {
		cout << "PlayerID " << playerID << "'s CombatInstance does not exist even though they are in combat." << endl;
		DatabaseTool::setCombatFlag(playerID, false, DatabaseTool::Target.character);
		return "Strangely, something seems to have failed in our combat system, your fight could not be found.\nYou have been removed from combat.\n";
	}
	instance->combatantRetreat(playerID);
	return "";
}

string Combat::acceptChallenge(int playerID, string arguments) {
	if (Combat::isInCombat(playerID)) {
		return "Finish your current fight before starting another, you barbarian.\n";
	}

	boost::trim(arguments)

	deque<std::string> parsedArgument;
	boost::split(parsedArgument, arguments, boost::is_any_of(" "));

	string enemyName = "";
	for (int parsedArgumentIndex = 0; parsedArgumentIndex < parsedArgument; parsedArgumentIndex++) {
		enemyName = parsedArgument.at(parsedArgumentIndex);
		if (enemyName.find_first_not_of(' ') != std::string::npos) {
			break;
		}
	}
	
	CombatInstance *instance = getCombatInstance(DatabaseTool::getCharName(enemyName));
	if (*instance == NULL) {
		return "The challenge timed out.\n";
	}
	if (!instance->inZone(DatabaseTool::getCharsLocation(playerID))) {
		return "You are no longer in the same zone as " + enemyName + "\n"; 
	}
	instance->acceptChallenge();
	return "";
}

bool Combat::isInCombat(int characterID, int characterType) {
	if (characterType == PLAYER_ONLY) {
		return DatabaseTool::inCombat(characterID, DatabaseTool::Target.character);
	}
	else if (characterType == NPC_ONLY) {
		return DatabaseTool::inCombat(characterID, DatabaseTool::Target.character);
	}
	return true;
}

// --------Public functions--------

string Combat::executeCommand(int playerID, Command givenCommand) {
	string command = givenCommand.type;
	string arguments = givenCommand.data;
	boost::to_lower(command);
	cout << command << " " << playerID << " " << arguments << endl;
	if (command == "fight") {
		return startCombat(playerID, arguments);
	}
	else if (command == "attack") {
		return queueAttack(playerID);
	}
	else if (command == "retreat") {
		return retreat(playerID);
	}
	else if (command == "acceptchallenge") {
		return acceptChallenge(playerID, arguments);
	}
	return "The command " + command + " was not recognized. Check help for a list of valid commands.\n";
}

bool Combat::isInCombat(int playerID) {
	return isInCombat(playerID, PLAYER_ONLY);
}

void Combat::endCombat(int playerID, string message) {
	CombatInstance *instance = getCombatInstance(playerID);
	if (*instance != NULL) {
		instance->endCombat(message);
		combatInstances.erase(std::remove(combatInstances.begin(), combatInstances.end(), *instance), combatInstances.end());
	}
}

void Combat::endCombat(int playerID) {
	endCombat(playerID, "Your combat has been ended prematurely.\n");
}

void Combat::endAllCombat(string message) {
	while (!combatInstances.empty()) {
		combatInstances.back().endCombat(message);
		combatInstances.pop_back();
	}
}

void Combat::endAllCombat() {
	endAllCombat("Your combat has been ended prematurely.\n");
}

void Combat::endAllCombat(int zoneID, string message) {
	for (long instanceIndex = (long)combatInstances.size() - 1; instanceIndex >= 0; instanceIndex--) {
		if (combatInstances.at(instanceIndex).inZone(zoneID) {
			combatInstances.at(instanceIndex).endCombat(message);
			combatInstances.erase(std::remove(combatInstances.begin(), combatInstances.end(), zoneInstances.at(instanceIndex)), combatInstances.end());
		}
	}
}

void Combat::endAllCombat(int zoneID) {
	endAllCombat(zoneID, "Your combat has been ended prematurely.\n");
}