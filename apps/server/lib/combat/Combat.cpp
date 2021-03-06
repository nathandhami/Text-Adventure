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
		return "Finish your current fight before starting another, you barbarian.";
	}
	if (DatabaseTool::getAttributes(playerID, Target::character).health <= 0) {
		return "You're no Black Knight, bring your health above 0 before trying to start a fight.";
	}

	deque<std::string> parsedArgument;
	boost::trim(arguments);
	boost::split(parsedArgument, arguments, boost::is_any_of(" "));
	
	Target enemyType = Target::null;
	string enemyName = "";
	for (int parsedArgumentIndex = 0; parsedArgumentIndex < parsedArgument.size(); parsedArgumentIndex++) {
		enemyName = parsedArgument.at(parsedArgumentIndex);
		if (enemyName.find_first_not_of(' ') != std::string::npos) {
			string disambiguityCheck = enemyName;
			boost::to_lower(disambiguityCheck);
			if (disambiguityCheck == "player") {
				enemyType = Target::character;
			}
			else if (disambiguityCheck == "npc") {
				enemyType = Target::npc;
			}
			else {
				break;
			}
		}
	}
	if (enemyName == "") {
		return "No need to take on the whole world, just fight one enemy at a time." + HOW_TO_START_FIGHT;
	}

	// This is pretty ugly, but I can't think of a cleaner way to do it
	int enemyPlayerID = 0;
	if (enemyType == Target::null || enemyType == Target::character) {
		enemyPlayerID = DatabaseTool::getCharIDFromName(enemyName);
		if (!DatabaseTool::isCharOnline(enemyPlayerID) || (DatabaseTool::getCharsLocation(playerID) != DatabaseTool::getCharsLocation(enemyPlayerID))) {
			enemyPlayerID = 0;
		}
	}
	int enemyNpcID = 0;
	if (enemyType == Target::null || enemyType == Target::npc) {
		enemyNpcID = DatabaseTool::getNpcInstanceIDFromName(enemyName, DatabaseTool::getCharsLocation(playerID));
	}

	if (enemyPlayerID > 0 && enemyNpcID > 0) {
		return "Do you want to attack the player named " + enemyName + " or the NPC named " + enemyName + "?\nPlease clarify by using one of these commands\n    fight player <name>\n    fight npc <name>";
	}
	else if (enemyPlayerID > 0) {
		if (playerID == enemyPlayerID) {
			return "You punch yourself in the face.";
		}
		if (Combat::isInCombat(enemyPlayerID, Target::character)) {
			return enemyName + " is already in combat.";
		}
		if (DatabaseTool::getAttributes(enemyPlayerID, Target::character).health <= 0) {
			return enemyName + " is dead, leave them be.";
		}
		combatInstancesLock.lock();
		combatInstances.push_back(std::make_shared<CombatInstance>(playerID, enemyPlayerID, Target::character, DatabaseTool::getCharsLocation(playerID)));
		combatInstancesLock.unlock();
	}
	else if (enemyNpcID > 0) {
		if (Combat::isInCombat(enemyNpcID, Target::npc)) {
			return enemyName + " is already in combat.";
		}
		if (DatabaseTool::getAttributes(enemyNpcID, Target::npc).health <= 0) {
			return enemyName + " is dead, leave them be.";
		}
		combatInstancesLock.lock();
		combatInstances.push_back(std::make_shared<CombatInstance>(playerID, enemyNpcID, Target::npc, DatabaseTool::getCharsLocation(playerID)));
		combatInstancesLock.unlock();
	}
	else {
		return "There is no " + enemyName + " in your zone.";
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
		return "Unfortunately, something seems to have failed in our combat system, your fight could not be found.\nYou have been removed from combat.";
	}
	return instance->queuePlayerAction(playerID, ATTACK_ACTION);
}

string Combat::retreat(int playerID) {
	if (!Combat::isInCombat(playerID)) {
		return "Calm down, you're not even in combat.";
	}
	std::shared_ptr<CombatInstance> instance = Combat::getCombatInstance(playerID);
	if (instance == NULL) {
		cout << "PlayerID " << playerID << "'s CombatInstance does not exist even though they are in combat." << endl;
		DatabaseTool::setCombatFlag(playerID, false, Target::character);
		return "Strangely, something seems to have failed in our combat system, your fight could not be found.\nYou have been removed from combat.";
	}
	return instance->queuePlayerAction(playerID, RETREAT_ACTION);
}

string Combat::respondToChallenge(int playerID, string command, string arguments) {
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
		return "You must specify the opponent's name, not your own.";
	}
	std::shared_ptr<CombatInstance> instance = Combat::getCombatInstance(playerID, opponentID);
	if (instance == NULL) {
		return "You have no active challenges with " + enemyName + ".";
	}

	if (command == "accept challenge") {
		if (!instance->isInitiator(playerID)) {
			Combat::isolateCombatInstance(playerID, opponentID);
			instance->acceptChallenge();
		}
		else {
			return "Patience, you have to let THEM accept YOUR challenge.";
		}
	}
	else if (command == "reject challenge") {
		if (!instance->isInitiator(playerID)) {
			instance->rejectChallenge();
		}
		else {
			return "If you want to retract your challenge to " + enemyName + " use the following command\n    retract challenge " + enemyName;
		}
	}
	else if (command == "retract challenge") {
		if (instance->isInitiator(playerID)) {
			instance->retractChallenge();
		}
		else {
			return "If you want to reject " + enemyName + "'s challenge use the following command\n    reject challenge " + enemyName;
		}
	}
	return "";
}

bool Combat::isInCombat(int characterID, Target characterType) {
	if (characterType == Target::character) {
		return DatabaseTool::inCombat(characterID, Target::character);
	}
	else if (characterType == Target::npc) {
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
	return "The command " + command + " was not recognized. Check help for a list of valid commands.";
}

string Combat::queueSpell(int playerID, Spell *currentSpell) {
	if (!Combat::isInCombat(playerID)) {
		return "For some strange reason your spell was queued as an in-combat attack, yet you are not in combat.\nWe cannot trust the source of your spell queueing, your spell has been cancelled.\nWe apologise for the inconvenience.";
	}
	std::shared_ptr<CombatInstance> instance = Combat::getCombatInstance(playerID);
	if (instance == NULL) {
		cout << "PlayerID " << playerID << "'s CombatInstance does not exist even though they are in combat." << endl;
		DatabaseTool::setCombatFlag(playerID, false, Target::character);
		return "Unfortunately, something seems to have failed in our combat system, your fight could not be found.\nYou have been removed from combat.";
	}
	return instance->queuePlayerAction(playerID, CAST_SPELL_ACTION, currentSpell);
}

bool Combat::isInCombat(int playerID) {
	return Combat::isInCombat(playerID, Target::character);
}

bool Combat::getOpponent(int playerID, int *opponentID, Target *opponentType) {
	std::shared_ptr<CombatInstance> combatInstance = Combat::getCombatInstance(playerID);
	if (combatInstance != NULL) {
		combatInstance->getOpponent(playerID, opponentID, opponentType);
		return true;
	}
	return false;
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
	Combat::endCombat(playerID, "Your combat has been ended prematurely.");
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
	Combat::endAllCombat("Your combat has been ended prematurely.");
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
	Combat::endAllCombat(zoneID, "Your combat has been ended prematurely.");
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