#include "CombatInstance.hpp"

// --------Private variables--------




// --------Private functions--------

Target getTargetFromType(int characterType) {
	if (characterType == PLAYER_ONLY) {
		return Target::character;
	}
	else if (characterType == NPC_ONLY) {
		return Target::npc;
	}
	return NULL;
}

void CombatInstance::removePlayerFromCombat(int playerID, int playerType, string message = "") {
	if (playerType == PLAYER_ONLY) {
		DatabaseTool::setCombatFlag(playerID, false, Target::character);
		Server::sendMessageToCharacter(playerID, GameCode::COMBAT, message);
	}
	else if (playerType == NPC_ONLY) {
		DatabaseTool::setCombatFlag(playerID, false, Target::npc);
	}
}

void CombatInstance::removePlayersFromCombat(string message = "") {
	CombatInstance::removePlayerFromCombat(playerOneID, PLAYER_ONLY, message);
	if (playerTwoPresent) {
		CombatInstance::removePlayerFromCombat(playerTwoID, enemyType, message);
		playerTwoPresent = false;
	}
}

void CombatInstance::pushPlayerAction(int player, int action) {
	if ((playersActionQueue.at(player).size() == 1) || action == RETREAT_ACTION) {
		playersActionQueue.at(player).push_front(action);
	}
	else {
		playersActionQueue.at(player).push_back(action);
	}	
}

void CombatInstance::waitForChallengeAccept() {
	string challengerName = DatabaseTool::getCharNameFromID(playerOneID);
	string challengeeName = DatabaseTool::getCharNameFromID(playerTwoID);
	Server::sendMessageToCharacter(playerTwoID, GameCode::COMBAT, challengerName + " has challenged you to a fight!\nTo respond to their challenge, use one of the following commands\n    accept challenge " + challengerName + "\n    reject challenge " + challengerName + "\n");
	int timeoutCounter = 0;
	string helpText = "To retract your challenge use the following command\n    retract challenge " + challengeeName + "\n";

	while (!challengeAccepted && timeoutCounter < CHALLENGE_TIMEOUT && keepFighting) {
		if ((timeoutCounter % 5) == 0) {
			Server::sendMessageToCharacter(playerOneID, GameCode::COMBAT, "Waiting for " + challengeeName + " to accept challenge...\n" + helpText);
			helpText = "";
		}
		sleep(HEARTBEAT_SECONDS);
		timeoutCounter++;
	}

	if (challengeAccepted) {
		DatabaseTool::setCombatFlag(playerTwoID, true, Target::character);
		playerTwoPresent = true;
		Server::sendMessageToCharacter(playerOneID, GameCode::COMBAT, challengeeName + " accepted your challenge!\nYou each bow respectfully and combat begins!\n");
		Server::sendMessageToCharacter(playerTwoID, GameCode::COMBAT, "You each bow respectfully and combat begins!\n");
		Zone::broadcastMessage(DatabaseTool::getCharsLocation(playerOneID), challengerName + " and " + challengeeName + " have started fighting each other!\n", vector<int>(playerOneID, playerTwoID));
	}
	else if (keepFighting) {
		keepFighting = false;
		Server::sendMessageToCharacter(playerOneID, GameCode::COMBAT, challengeeName + " did not accept your challenge.\n");
		Server::sendMessageToCharacter(playerTwoID, GameCode::COMBAT, challengerName + " has given up waiting for you to respond to their challenge.\n");
	}
}

void CombatInstance::notifyAttack(int player, int characterType, int damageDealt, int healthRemaining) {
	if (player == PLAYER_ONE) {
		Server::sendMessageToCharacter(playerOneID, GameCode::COMBAT, "You deal " + std::to_string(damageDealt) + " damage.\n");
		if (characterType == PLAYER_ONLY) {
			Server::sendMessageToCharacter(playerTwoID, GameCode::COMBAT, "You take " + std::to_string(damageDealt) + " damage. You have " + std::to_string(healthRemaining) + "hp.\n");
		}
	}
	else {
		Server::sendMessageToCharacter(playerOneID, GameCode::COMBAT, "You take " + std::to_string(damageDealt) + " damage. You have " + std::to_string(healthRemaining) + "hp.\n");
		if (characterType == PLAYER_ONLY) {
			Server::sendMessageToCharacter(playerTwoID, GameCode::COMBAT, "You deal " + std::to_string(damageDealt) + " damage.\n");
		}
	}
}

void CombatInstance::playerWin(int playerID) {
	keepFighting = false;
	if ((playerID != playerTwoID) || (enemyType != NPC_ONLY)) {
		Server::sendMessageToCharacter(playerID, GameCode::COMBAT, VICTORY_NOTIFICATION);
	}
}

void CombatInstance::playerLose(int playerID) {
	keepFighting = false;
	if ((playerID == playerTwoID) && (enemyType == NPC_ONLY)) {
		DatabaseTool::murderNpc(playerTwoID);
	}
	else {
		Server::sendMessageToCharacter(playerID, GameCode::COMBAT, DEFEAT_NOTIFICATION);
	}
}

// Waiting on equipment
void CombatInstance::executePlayerAttack(int attacker, int characterType) {
	Target attackerTarget = getTargetFromType(characterType);
	Attributes attackerAttributes = DatabaseTool::getAttributes(attacker, attackerTarget);
	int damageDealt = -attackerAttributes.strength * 2;    // Until we have weapons and armour we'll just do this for damage

	Target defenderTarget;
	int defender = 0;
	
	if (attacker == PLAYER_ONE) {
		defenderTarget = getTargetFromType(enemyType);
		defender = playerTwoID;
		/*
		playerTwoHealthRemaining -= damageDealt;
		notifyAttack(player, characterType, damageDealt, playerTwoHealthRemaining);
		if (playerTwoHealthRemaining <= 0) {
			keepFighting = false;
			playerWin(playerOneID);
			playerLose(playerTwoID);
		}
		*/
	}
	else {
		defenderTarget = getTargetFromType(PLAYER_ONLY);
		defender = playerOneID;
		/*
		playerOneHealthRemaining -= damageDealt;
		notifyAttack(player, characterType, damageDealt, playerOneHealthRemaining);
		if (playerOneHealthRemaining <= 0) {
			keepFighting = false;
			playerWin(playerTwoID);
			playerLose(playerOneID);
		}
		*/
	}

	Attributes defenderAttributesModifier = Attributes();
	defenderAttributesModifier.id = defender;
	defenderAttributesModifier.health = damageDealt;  
	DatabaseTool::updateAttributes(defenderAttributesModifier, defenderTarget);

	int healthRemaining = DatabaseTool::getAttributes(defender, defenderTarget).health;
	notifyAttack(attacker, characterType, damageDealt, healthRemaining);

	if (healthRemaining <= 0) {
		playerWin(attacker);
		playerLose(defender);
	}

	/*
	Attributes attackerAttributes;
	Attributes defenderAttributes;

	
	if (player == PLAYER_ONE) {
		damageDealt = 
		if (characterType == PLAYER_ONLY) {
			healthRemaining = DatabaseTool::getCharHealth(playerTwoID);
		}
		else {
			healthRemaining = DatabaseTool::getNpcHealth(playerTwoID);
		}
	}
	else {
		if (characterType == PLAYER_ONLY) {
			damageDealt = // Get player damage
		}
		else {
			damageDealt = // Get npc damage
		}
		healthRemaining = DatabaseTool::getCharHealth(playerOneID);
	}
	*/	
}

void CombatInstance::executePlayerRetreat(int player) {
	keepFighting = false;
	if (player == PLAYER_ONE && enemyType == PLAYER_ONLY) {
		if (DatabaseTool::getAttributes(player, getTargetFromType(PLAYER_ONLY)).health > 0) {
			Server::sendMessageToCharacter(playerOneID, GameCode::COMBAT, "You run away.");
			Server::sendMessageToCharacter(playerTwoID, GameCode::COMBAT, RETREAT_NOTIFICATION);
		}
	}
	else if (player == PLAYER_TWO) {
		if (DatabaseTool::getAttributes(player, getTargetFromType(enemyType)).health > 0) {
			Server::sendMessageToCharacter(playerTwoID, GameCode::COMBAT, "You run away.");
			Server::sendMessageToCharacter(playerOneID, GameCode::COMBAT, RETREAT_NOTIFICATION);
		}
	}
}

void CombatInstance::executePlayerAction(int player, int characterType) {
	if (playersActionQueue.at(player).empty()) {
		playersActionQueue.at(player).push_back(ATTACK_ACTION);
	}

	if (playersActionQueue.at(player).front() == ATTACK_ACTION) {
		CombatInstance::executePlayerAttack(player, characterType);
	}
	else if (playersActionQueue.at(player).front() == RETREAT_ACTION) {
		CombatInstance::executePlayerRetreat(player);
	}

	playersActionQueue.at(player).pop_front();
}

void CombatInstance::runCombat() {
	if (enemyType == PLAYER_ONLY) {
		CombatInstance::waitForChallengeAccept();
	}
	else {
		DatabaseTool::setCombatFlag(playerTwoID, true, Target::npc);
		playerTwoPresent = true;
		challengeAccepted = true;   // Unnecessary, but it would annoy me if we were fighting with challengeAccepted being false =P
		string challengerName = DatabaseTool::getCharNameFromID(playerOneID);
		string npcName = DatabaseTool::getNpcName(playerTwoID);
		Zone::broadcastMessage(DatabaseTool::getCharsLocation(playerOneID), challengerName + " has started fighting " + npcName + "!\n", vector<int>(playerOneID));
	}

	playersActionQueue.push_back(deque<int> (1, ATTACK_ACTION));
	playersActionQueue.push_back(deque<int> (1, ATTACK_ACTION));

	while (keepFighting) {
		CombatInstance::executePlayerAction(PLAYER_ONE, PLAYER_ONLY);
		CombatInstance::executePlayerAction(PLAYER_TWO, enemyType);
		sleep(HEARTBEAT_SECONDS);
	}
}

// --------Public functions--------

bool CombatInstance::isCombatant(int playerID) {
	if ((playerOneID == playerID) || ((enemyType == PLAYER_ONLY) && (playerTwoID == playerID))) {
		return true;
	}
	return false;
}

bool CombatInstance::isInitiator(int playerID) {
	return (playerID == playerOneID);
}

bool CombatInstance::inZone(int zoneID) {
	if (zoneID == combatZoneID) {
		return true;
	}
	return false;
}

bool CombatInstance::isStillFighting() {
	return keepFighting;
}

bool CombatInstance::isReadyForCleanup() {
	return readyForCleanup;
}

void CombatInstance::queuePlayerAction(int playerID, int action) {
	if (playerID == playerOneID) {
		pushPlayerAction(PLAYER_ONE, action);
	}
	else if (playerID == playerTwoID) {
		pushPlayerAction(PLAYER_TWO, action);
	}
}

void CombatInstance::acceptChallenge() {
	challengeAccepted = true;
}

void CombatInstance::rejectChallenge() {
	if (!challengeAccepted) {
		Server::sendMessageToCharacter(playerTwoID, GameCode::COMBAT, "You calmly explain to " + DatabaseTool::getCharNameFromID(playerOneID) + " that you do not want to fight at the moment.\n");
		Server::sendMessageToCharacter(playerOneID, GameCode::COMBAT, DatabaseTool::getCharNameFromID(playerTwoID) + " tells you you're not worthy to fight him.\n");
		CombatInstance::endCombat("");
	}
}

void CombatInstance::retractChallenge() {
	if (!challengeAccepted) {
		Server::sendMessageToCharacter(playerOneID, GameCode::COMBAT, "You decide you don't really want to fight " + DatabaseTool::getCharNameFromID(playerTwoID) + ", so you retract the challenge.\n");
		Server::sendMessageToCharacter(playerTwoID, GameCode::COMBAT, DatabaseTool::getCharNameFromID(playerOneID) + " retracts his challenge.\n");
		CombatInstance::endCombat("");
	}
}

void CombatInstance::endCombat(string message) {
	if (!endingCombat) {
		endingCombat = true;
		keepFighting = false;
		combatThread.join();
		CombatInstance::removePlayersFromCombat(message);
		readyForCleanup = true;
	}
}

void CombatInstance::playerDisconnect(int playerID) {
	int opponentID = playerOneID;
	if (playerID == playerOneID) {
		opponentID = playerTwoID;
	}
	if (!challengeAccepted) {
		Server::sendMessageToCharacter(opponentID, GameCode::COMBAT, DatabaseTool::getCharNameFromID(playerID) + " disconnected, so don't worry about responding to their challenge.\n");
		CombatInstance::endCombat("");
	}
	else if (keepFighting) {
		Server::sendMessageToCharacter(opponentID, GameCode::COMBAT, DatabaseTool::getCharNameFromID(playerID) + " disconnected.\n");
		playerWin(opponentID);
		CombatInstance::endCombat("");	
	}
}

CombatInstance::CombatInstance(int playerID, int enemyID, int givenEnemyType, int zoneID) {
	DatabaseTool::setCombatFlag(playerID, true, Target::character);

	combatZoneID = zoneID;
	enemyType = givenEnemyType;
	playerOneID = playerID;
	playerTwoID = enemyID;

	combatThread = std::thread(&CombatInstance::runCombat, this);
}

CombatInstance::~CombatInstance() {
	if (!readyForCleanup) {
		endCombat("Your fight has been ended prematurely.\n");
	}
}