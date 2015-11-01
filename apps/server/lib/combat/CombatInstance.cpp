#include "CombatInstance.hpp"


// --------Private functions--------

void CombatInstance::removePlayersFromCombat(int playerID, int playerType) {
	if (playerType == PLAYER_ONLY) {
		DatabaseTool::setCombatFlag(playerID, false, DatabaseTool::Target.character);
	}
	else if (playerType == NPC_ONLY) {
		DatabaseTool::setCombatFlag(playerID, false, DatabaseTool::Target.npc);
	}
}

void CombatInstance::removePlayersFromCombat() {
	removePlayersFromCombat(playerOneID, PLAYER_ONLY);
	removePlayersFromCombat(playerTwoID, enemyType);
}

void waitForChallengeAccept() {
	string challengerName = DatabaseTool::getCharNameFromID(playerOneID);
	string challengeeName = DatabaseTool::getCharNameFromID(playerTwoID);
	Server::sendMessageToCharacter(playerTwoID, GameCode::Combat, challengerName + " has challenged you to a fight!\nTo accept their challenge, use the following command\n    acceptChallenge " + challengerName + "\n");
	int timeoutCounter = 0;

	while (!challengeAccepted && timeoutCounter < CHALLENGE_TIMEOUT && keepFighting) {
		if ((timeoutCounter % 5) == 0) {
			Server::sendMessageToCharacter(playerOneID, GameCode::Combat, "Waiting for " + challengeeName + " to accept challenge...\n");
		}
		sleep(HEARTBEAT_SECONDS);
		timeoutCounter++;
	}

	if (challengeAccepted) {
		DatabaseTool::setCombatFlag(playerID, true, DatabaseTool::Target.character);
		Server::sendMessageToCharacter(playerOneID, GameCode::Combat, challengeeName + " accepted your challenge!\nYou each bow respectfully and combat begins!\n");
		Server::sendMessageToCharacter(playerTwoID, GameCode::Combat, "You each bow respectfully and combat begins!\n");
		Zone::broadcastMessage(DatabaseTool::getCharsLocation(playerOneID), challengerName + " and " + challengeeName + " have started fighting each other!\n");
	}
	else if (keepFighting) {
		Server::sendMessageToCharacter(playerOneID, GameCode::Combat, challengeeName + " did not accept your challenge.\n");
		removePlayersFromCombat(playerOneID, PLAYER_ONLY);
		keepFighting = false;
		readyForCleanup = true;
		pthread_exit(NULL);
	}
}

void CombatInstance::notifyAttack(int player, int characterType, int damageDealt, int healthRemaining) {
	if (player == PLAYER_ONE) {
		Server::sendMessageToCharacter(playerOneID, GameCode::Combat, "You deal " + damageDealt + " damage.\n");
		if (healthRemaining <= 0) {
			Server::sendMessageToCharacter(playerOneID, GameCode::Combat, VICTORY_NOTIFICATION);
		}
		if (characterType == PLAYER_ONLY) {
			Server::sendMessageToCharacter(playerTwoID, GameCode::Combat, "You take " + damageDealt + " damage. You have " + healthRemaining + "hp.\n");
			if (healthRemaining <= 0) {
				Server::sendMessageToCharacter(playerTwoID, GameCode::Combat, DEFEAT_NOTIFICATION);
			}
		}
	}
	else {
		Server::sendMessageToCharacter(playerOneID, GameCode::Combat, "You take " + damageDealt + " damage. You have " + healthRemaining + "hp.\n");
		if (healthRemaining <= 0) {
			Server::sendMessageToCharacter(playerOneID, GameCode::Combat, DEFEAT_NOTIFICATION);
		}
		if (characterType == PLAYER_ONLY) {
			Server::sendMessageToCharacter(playerTwoID, GameCode::Combat, "You deal " + damageDealt + " damage.\n");
			if (healthRemaining <= 0) {
				Server::sendMessageToCharacter(playerTwoID, GameCode::Combat, VICTORY_NOTIFICATION);
			}
		}
	}
}

void CombatInstance::executePlayerAttack(int player, int characterType) {
	/*int damageDealt = 0;
	int healthRemaining = 0;
	if (player == PLAYER_ONE) {
		damageDealt = // Get player damage
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
	healthRemaining = healthRemaining - damageDealt;
	// Deal player damage to other player health
	if (healthRemaining <= 0) {
		keepFighting = false;
	}
	notifyAttack(player, characterType, damageDealt, healthRemaining);
	*/
	notifyAttack(player, characterType, 0, 50);

}

void CombatInstance::executePlayerRetreat(int player) {
	keepFighting = false;
	if (player == PLAYER_ONE && enemyType == PLAYER_ONLY) {
		Server::sendMessageToCharacter(playerTwoID, GameCode::Combat, RETREAT_NOTIFICATION);
	}
	else {
		Server::sendMessageToCharacter(playerOneID, GameCode::Combat, RETREAT_NOTIFICATION);
	}
}

void CombatInstance::executePlayerAction(int player, int characterType) {
	if (playersActionQueue.at(player).front() == ATTACK_ACTION) {
		executePlayerAttack(player, characterType);
	}
	else if (playersActionQueue.at(player).front() == RETREAT_ACTION) {
		executePlayerRetreat(player);
	}

	if (playersActionQueue.at(player).size() > 1) {
		playersActionQueue.at(player).pop_front();
	}
}

void *CombatInstance::runCombat(void* args) {
	if (enemyType == PLAYER_ONLY) {
		waitForChallengeAccept();
	}
	else {
		DatabaseTool::setCombatFlag(playerID, true, DatabaseTool::Target.npc);
		challengeAccepted = true;   // Unnecessary, but it would annoy me if we were fighting with challengeAccepted being false =P
		string challengerName = DatabaseTool::getCharNameFromID(playerOneID);
		string npcName = DatabaseTool::getNpcName(playerTwoID);
		Zone::broadcastMessage(DatabaseTool::getCharsLocation(playerOneID), challengerName + "has started fighting " + npcName + "!\n");
	}

	playersActionQueue.push_back(deque<int> (1, ATTACK_ACTION));
	playersActionQueue.push_back(deque<int> (1, ATTACK_ACTION));

	while (keepFighting) {
		executePlayerAction(PLAYER_ONE, PLAYER_ONLY);
		executePlayerAction(PLAYER_TWO, enemyType);
		sleep(HEARTBEAT_SECONDS);
	}

	removePlayersFromCombat();

	readyForCleanup = true;
	pthread_exit(NULL);
}

// --------Public functions--------

bool CombatInstance::isCombatant(int playerID) {
	if ((playerOneID == playerID) || ((enemyType == PLAYER_ONLY) && (playerTwoID == playerID))) {
		return true;
	}
	return false;
}

bool CombatInstance::inZone(int zoneID) {
	if (zoneID == combatZoneID) {
		return true;
	}
	return false;
}

void CombatInstance::acceptChallenge() {
	challengeAccepted = true;
}

void CombatInstance::endCombat(string message) {
	if (keepFighting) {
		keepFighting = false;
		pthread_join(combatThread);
		removePlayersFromCombat(playerOneID, PLAYER_ONLY);
		removePlayersFromCombat(playerTwoID, enemyType);
		Server::sendMessageToCharacter(playerOneID, GameCode::Combat, message);
		if (enemyType == PLAYER_ONLY) {
			Server::sendMessageToCharacter(playerTwoID, GameCode::Combat, message);
		}
	}
	readyForCleanup = true;
}

CombatInstance::CombatInstance(int playerID, int enemyID, int givenEnemyType, int zoneID) {
	DatabaseTool::setCombatFlag(playerID, true, DatabaseTool::Target.character);

	combatZoneID = zoneID;
	enemyType = givenEnemyType;
	playerOneID = playerID;
	playerTwoID = enemyID;

	int errorStartingThread = pthread_create(&combatThread, NULL, runCombat, NULL);
	if (errorStartingThread) {
		cout << "Failed to create combat thread: " << errorStartingThread << "\n";
		exit(-1);
	}

	pthread_exit(NULL);
}

CombatInstance::~CombatInstance() {
	if (!readyForCleanup) {
		endCombat("Your fight has been ended prematurely.\n");
	}
}