#include "Combat.hpp"


// --------Private variables--------

vector<std::shared_ptr<CombatInstance>> Combat::combatInstances;

bool Combat::cleanupThreadRunning = false;

thread Combat::cleanupThread;

mutex Combat::combatInstancesLock;

// --------Private functions--------

void Combat::cleanupCombats() {
	while (true) {
		combatInstancesLock.lock();
		for (long instanceIndex = (long)combatInstances.size() - 1; instanceIndex >= 0; instanceIndex--) {
			if (!combatInstances[instanceIndex]->isStillFighting()) {
				combatInstances[instanceIndex]->endCombat("");
				if (combatInstances[instanceIndex]->isReadyForCleanup()) {
					combatInstances.erase(combatInstances.begin() + instanceIndex);
				}
			}
		}
		combatInstancesLock.unlock();
	}
}

void Combat::startCombatThread() {
	if (!cleanupThreadRunning) {
		cleanupThread = std::thread(&Combat::cleanupCombats);
		cleanupThreadRunning = true;
	}
}

std::shared_ptr<CombatInstance> Combat::getCombatInstance(int playerID, int opponentID) {
	combatInstancesLock.lock();
	for (long instanceIndex = 0; instanceIndex < combatInstances.size(); instanceIndex++) {
		if ((opponentID == -1) || combatInstances[instanceIndex]->isCombatant(opponentID)) {
			if (combatInstances[instanceIndex]->isCombatant(playerID)) {
				combatInstancesLock.unlock();
				return combatInstances[instanceIndex];
			}
		}
	}
	combatInstancesLock.unlock();
	return NULL;
}

std::shared_ptr<CombatInstance> Combat::getCombatInstance(int playerID) {
	return Combat::getCombatInstance(playerID, -1);
}

void Combat::isolateCombatInstance(int playerID, int opponentID) {
	combatInstancesLock.lock();
	for (long instanceIndex = 0; instanceIndex < combatInstances.size(); instanceIndex++) {
		if (combatInstances[instanceIndex]->isCombatant(playerID)) {
			if (!combatInstances[instanceIndex]->isCombatant(opponentID)) {
				combatInstances[instanceIndex]->rejectChallenge();
			}
		}
		else if (combatInstances[instanceIndex]->isCombatant(opponentID)) {
			if (!combatInstances[instanceIndex]->isCombatant(playerID)) {
				combatInstances[instanceIndex]->rejectChallenge();	
			}
		}

	}
	combatInstancesLock.unlock();
}

// Violates the idea of a function not taking many lines
string Combat::startCombat(int playerID, string arguments) {
	if (Combat::isInCombat(playerID)) {
		return "Finish your current fight before starting another, you barbarian.\n";
	}

	deque<std::string> parsedArgument;
	boost::trim(arguments);
	boost::split(parsedArgument, arguments, boost::is_any_of(" "));
	
	int enemyType = PLAYER_OR_NPC;
	string enemyName = "";
	for (int parsedArgumentIndex = 0; parsedArgumentIndex < parsedArgument.size(); parsedArgumentIndex++) {
		enemyName = parsedArgument.at(parsedArgumentIndex);
		if (enemyName.find_first_not_of(' ') != std::string::npos) {
			string disambiguityCheck = enemyName;
			boost::to_lower(disambiguityCheck);
			if (disambiguityCheck == "player") {
				enemyType = PLAYER_ONLY;
			}
			else if (disambiguityCheck == "npc") {
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
	if (enemyType == PLAYER_OR_NPC || enemyType == PLAYER_ONLY) {
		enemyPlayerID = DatabaseTool::getCharIDFromName(enemyName);
		if (!DatabaseTool::isCharOnline(enemyPlayerID) || (DatabaseTool::getCharsLocation(playerID) != DatabaseTool::getCharsLocation(enemyPlayerID))) {
			enemyPlayerID = 0;
		}
	}
	int enemyNpcID = 0;
	if (enemyType == PLAYER_OR_NPC || enemyType == NPC_ONLY) {
		enemyNpcID = DatabaseTool::getNpcInstanceIDFromName(enemyName, DatabaseTool::getCharsLocation(playerID));
	}

	if (enemyPlayerID > 0 && enemyNpcID > 0) {
		return "Do you want to attack the player named " + enemyName + " or the NPC named " + enemyName + "?\nPlease clarify by using one of these commands\n    fight player <name>\n    fight npc <name>\n";
	}
	else if (enemyPlayerID > 0) {
		if (playerID == enemyPlayerID) {
			return "You punch yourself in the face.\n";
		}
		if (Combat::isInCombat(enemyPlayerID, PLAYER_ONLY)) {
			return enemyName + " is already in combat.\n";
		}
		combatInstancesLock.lock();
		combatInstances.push_back(std::make_shared<CombatInstance>(playerID, enemyPlayerID, PLAYER_ONLY, DatabaseTool::getCharsLocation(playerID)));
		combatInstancesLock.unlock();
	}
	else if (enemyNpcID > 0) {
		if (Combat::isInCombat(enemyNpcID, NPC_ONLY)) {
			return enemyName + " is already in combat.\n";
		}
		combatInstancesLock.lock();
		combatInstances.push_back(std::make_shared<CombatInstance>(playerID, enemyNpcID, NPC_ONLY, DatabaseTool::getCharsLocation(playerID)));
		combatInstancesLock.unlock();
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
	std::shared_ptr<CombatInstance> instance = Combat::getCombatInstance(playerID);
	if (instance == NULL) {
		cout << "PlayerID " << playerID << "'s CombatInstance does not exist even though they are in combat." << endl;
		DatabaseTool::setCombatFlag(playerID, false, Target::character);
		return "Unfortunately, something seems to have failed in our combat system, your fight could not be found.\nYou have been removed from combat.\n";
	}
	instance->queuePlayerAction(playerID, ATTACK_ACTION);
	return "";
}

string Combat::retreat(int playerID) {
	if (!Combat::isInCombat(playerID)) {
		return "Calm down, you're not even in combat.\n";
	}
	std::shared_ptr<CombatInstance> instance = Combat::getCombatInstance(playerID);
	if (instance == NULL) {
		cout << "PlayerID " << playerID << "'s CombatInstance does not exist even though they are in combat." << endl;
		DatabaseTool::setCombatFlag(playerID, false, Target::character);
		return "Strangely, something seems to have failed in our combat system, your fight could not be found.\nYou have been removed from combat.\n";
	}
	instance->queuePlayerAction(playerID, RETREAT_ACTION);
	return "";
}

string Combat::respondToChallenge(int playerID, string command, string arguments) {
	/*if (Combat::isInCombat(playerID)) {
		return "Finish your current fight before starting another, you barbarian.\n";
	}*/

	boost::trim(arguments);

	deque<std::string> parsedArgument;
	boost::split(parsedArgument, arguments, boost::is_any_of(" "));

	string enemyName = "";
	for (int parsedArgumentIndex = 0; parsedArgumentIndex < parsedArgument.size(); parsedArgumentIndex++) {
		enemyName = parsedArgument.at(parsedArgumentIndex);
		if (enemyName.find_first_not_of(' ') != std::string::npos) {
			break;
		}
	}
	
	int opponentID = DatabaseTool::getCharIDFromName(enemyName);
	if (playerID == opponentID) {
		return "You must specify the opponent's name, not your own.\n";
	}
	std::shared_ptr<CombatInstance> instance = Combat::getCombatInstance(playerID, opponentID);
	if (instance == NULL) {
		return "You have no active challenges with " + enemyName + ".\n";
	}

	if (command == "accept challenge") {
		if (!instance->isInitiator(playerID)) {
			Combat::isolateCombatInstance(playerID, opponentID);
			instance->acceptChallenge();
		}
		else {
			return "Patience, you have to let THEM accept YOUR challenge\n";
		}
	}
	else if (command == "reject challenge") {
		if (!instance->isInitiator(playerID)) {
			instance->rejectChallenge();
		}
		else {
			return "If you want to retract your challenge to " + enemyName + " use the following command\n    retract challenge " + enemyName + "\n";
		}
	}
	else if (command == "retract challenge") {
		if (instance->isInitiator(playerID)) {
			instance->retractChallenge();
		}
		else {
			return "If you want to reject " + enemyName + "'s challenge use the following command\n    reject challenge " + enemyName + "\n";
		}
	}
	return "";
}

bool Combat::isInCombat(int characterID, int characterType) {
	if (characterType == PLAYER_ONLY) {
		return DatabaseTool::inCombat(characterID, Target::character);
	}
	else if (characterType == NPC_ONLY) {
		return DatabaseTool::inCombat(characterID, Target::npc);
	}
	return true;
}

// --------Public functions--------

string Combat::executeCommand(int playerID, Command givenCommand) {
	startCombatThread();
	string command = givenCommand.type;
	string arguments = givenCommand.data;
	boost::to_lower(command);
	cout << command << " " << playerID << " " << arguments << endl;
	if (command == "fight") {
		return Combat::startCombat(playerID, arguments);
	}
	else if (command == "attack") {
		return Combat::queueAttack(playerID);
	}
	else if (command == "retreat") {
		return Combat::retreat(playerID);
	}
	else if (command == "accept challenge" || command == "reject challenge" || command == "retract challenge") {
		return Combat::respondToChallenge(playerID, command, arguments);
	}
	return "The command " + command + " was not recognized. Check help for a list of valid commands.\n";
}

bool Combat::isInCombat(int playerID) {
	return Combat::isInCombat(playerID, PLAYER_ONLY);
}

void Combat::endCombat(int playerID, string message) {	
	combatInstancesLock.lock();
	for (long instanceIndex = (long)combatInstances.size() - 1; instanceIndex >= 0; instanceIndex--) {
		if (combatInstances[instanceIndex]->isCombatant(playerID)) {
			combatInstances[instanceIndex]->endCombat(message);
			combatInstances.erase(combatInstances.begin() + instanceIndex);
		}
	}
	combatInstancesLock.unlock();
}

void Combat::endCombat(int playerID) {
	Combat::endCombat(playerID, "Your combat has been ended prematurely.\n");
}

void Combat::endAllCombat(string message) {
	cout << "ENDING ALL COMBAT EVERYWHERE" << endl;
	combatInstancesLock.lock();
	while (!combatInstances.empty()) {
		combatInstances.back()->endCombat(message);
		combatInstances.pop_back();
	}
	combatInstancesLock.unlock();
}

void Combat::endAllCombat() {
	Combat::endAllCombat("Your combat has been ended prematurely.\n");
}

void Combat::endAllCombat(int zoneID, string message) {
	cout << "ENDING ALL COMBAT IN ZONE " << zoneID << endl;
	// Have to do some silly stuff here to avoid race condition issues
	bool keepGoing = true;
	combatInstancesLock.lock();
	while (keepGoing) {
		for (long instanceIndex = (long)combatInstances.size() - 1; instanceIndex >= 0; instanceIndex--) {
			if (combatInstances[instanceIndex]->inZone(zoneID)) {
				combatInstances[instanceIndex]->endCombat(message);
				combatInstances.erase(combatInstances.begin() + instanceIndex);
				if (instanceIndex == 0) {
					keepGoing = false;
				}
				break;
			}
			if (instanceIndex <= 0) {
				keepGoing = false;
			}
		}
	}
	combatInstancesLock.unlock();
}

void Combat::endAllCombat(int zoneID) {
	Combat::endAllCombat(zoneID, "Your combat has been ended prematurely.\n");
}

void Combat::playerDisconnect(int playerID) {
	combatInstancesLock.lock();
	for (long instanceIndex = (long)combatInstances.size() - 1; instanceIndex >= 0; instanceIndex--) {
		if (combatInstances[instanceIndex]->isCombatant(playerID)) {
			combatInstances[instanceIndex]->playerDisconnect(playerID);
		}
	}
	combatInstancesLock.unlock();
}