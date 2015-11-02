#include "CombatInstance.hpp"

// --------Private variables--------

bool challengeAccepted = false;
bool keepFighting = true;
bool readyForCleanup = false;

int combatZoneID = 0;
int enemyType = 0;
int playerOneID = 0;
int playerTwoID = 0;

deque<deque<int>> playersActionQueue;


// --------Private functions--------

void CombatInstance::removePlayersFromCombat(int playerID, int playerType) {
	if (playerType == PLAYER_ONLY) {
		DatabaseTool::setCombatFlag(playerID, false, Target::character);
	}
	else if (playerType == NPC_ONLY) {
		DatabaseTool::setCombatFlag(playerID, false, Target::npc);
	}
}

void CombatInstance::removePlayersFromCombat() {
	CombatInstance::removePlayersFromCombat(playerOneID, PLAYER_ONLY);
	CombatInstance::removePlayersFromCombat(playerTwoID, enemyType);
}

void CombatInstance::waitForChallengeAccept() {
	string challengerName = DatabaseTool::getCharNameFromID(playerOneID);
	string challengeeName = DatabaseTool::getCharNameFromID(playerTwoID);
	Server::sendMessageToCharacter(playerTwoID, GameCode::COMBAT, challengerName + " has challenged you to a fight!\nTo accept their challenge, use the following command\n    acceptChallenge " + challengerName + "\n");
	int timeoutCounter = 0;

	while (!challengeAccepted && timeoutCounter < CHALLENGE_TIMEOUT && keepFighting) {
		if ((timeoutCounter % 5) == 0) {
			Server::sendMessageToCharacter(playerOneID, GameCode::COMBAT, "Waiting for " + challengeeName + " to accept challenge...\n");
		}
		sleep(HEARTBEAT_SECONDS);
		timeoutCounter++;
	}

	if (challengeAccepted) {
		DatabaseTool::setCombatFlag(playerTwoID, true, Target::character);
		Server::sendMessageToCharacter(playerOneID, GameCode::COMBAT, challengeeName + " accepted your challenge!\nYou each bow respectfully and combat begins!\n");
		Server::sendMessageToCharacter(playerTwoID, GameCode::COMBAT, "You each bow respectfully and combat begins!\n");
		Zone::broadcastMessage(DatabaseTool::getCharsLocation(playerOneID), challengerName + " and " + challengeeName + " have started fighting each other!\n");
	}
	else if (keepFighting) {
		Server::sendMessageToCharacter(playerOneID, GameCode::COMBAT, challengeeName + " did not accept your challenge.\n");
		CombatInstance::removePlayersFromCombat(playerOneID, PLAYER_ONLY);
		keepFighting = false;
		readyForCleanup = true;
		//pthread_exit(NULL);
	}
}

void CombatInstance::notifyAttack(int player, int characterType, int damageDealt, int healthRemaining) {
	if (player == PLAYER_ONE) {
		Server::sendMessageToCharacter(playerOneID, GameCode::COMBAT, "You deal " + std::to_string(damageDealt) + " damage.\n");
		if (healthRemaining <= 0) {
			Server::sendMessageToCharacter(playerOneID, GameCode::COMBAT, VICTORY_NOTIFICATION);
		}
		if (characterType == PLAYER_ONLY) {
			Server::sendMessageToCharacter(playerTwoID, GameCode::COMBAT, "You take " + std::to_string(damageDealt) + " damage. You have " + std::to_string(healthRemaining) + "hp.\n");
			if (healthRemaining <= 0) {
				Server::sendMessageToCharacter(playerTwoID, GameCode::COMBAT, DEFEAT_NOTIFICATION);
			}
		}
	}
	else {
		Server::sendMessageToCharacter(playerOneID, GameCode::COMBAT, "You take " + std::to_string(damageDealt) + " damage. You have " + std::to_string(healthRemaining) + "hp.\n");
		if (healthRemaining <= 0) {
			Server::sendMessageToCharacter(playerOneID, GameCode::COMBAT, DEFEAT_NOTIFICATION);
		}
		if (characterType == PLAYER_ONLY) {
			Server::sendMessageToCharacter(playerTwoID, GameCode::COMBAT, "You deal " + std::to_string(damageDealt) + " damage.\n");
			if (healthRemaining <= 0) {
				Server::sendMessageToCharacter(playerTwoID, GameCode::COMBAT, VICTORY_NOTIFICATION);
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
		Server::sendMessageToCharacter(playerTwoID, GameCode::COMBAT, RETREAT_NOTIFICATION);
	}
	else {
		Server::sendMessageToCharacter(playerOneID, GameCode::COMBAT, RETREAT_NOTIFICATION);
	}
}

void CombatInstance::executePlayerAction(int player, int characterType) {
	if (playersActionQueue.at(player).front() == ATTACK_ACTION) {
		CombatInstance::executePlayerAttack(player, characterType);
	}
	else if (playersActionQueue.at(player).front() == RETREAT_ACTION) {
		CombatInstance::executePlayerRetreat(player);
	}

	if (playersActionQueue.at(player).size() > 1) {
		playersActionQueue.at(player).pop_front();
	}
}

void CombatInstance::runCombat() {
	if (enemyType == PLAYER_ONLY) {
		CombatInstance::waitForChallengeAccept();
	}
	else {
		DatabaseTool::setCombatFlag(playerTwoID, true, Target::npc);
		challengeAccepted = true;   // Unnecessary, but it would annoy me if we were fighting with challengeAccepted being false =P
		string challengerName = DatabaseTool::getCharNameFromID(playerOneID);
		string npcName = DatabaseTool::getNpcName(playerTwoID);
		Zone::broadcastMessage(DatabaseTool::getCharsLocation(playerOneID), challengerName + "has started fighting " + npcName + "!\n");
	}

	playersActionQueue.push_back(deque<int> (1, ATTACK_ACTION));
	playersActionQueue.push_back(deque<int> (1, ATTACK_ACTION));

	while (keepFighting) {
		CombatInstance::executePlayerAction(PLAYER_ONE, PLAYER_ONLY);
		CombatInstance::executePlayerAction(PLAYER_TWO, enemyType);
		sleep(HEARTBEAT_SECONDS);
	}

	CombatInstance::removePlayersFromCombat();

	readyForCleanup = true;
	//pthread_exit(NULL);
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

void CombatInstance::queuePlayerAction(int playerID, int action) {
	if (playerID == playerOneID) {
		playersActionQueue.at(PLAYER_ONE).push_back(action);
	}
	else if (playerID == playerTwoID) {
		playersActionQueue.at(PLAYER_TWO).push_back(action);
	}
}

void CombatInstance::acceptChallenge() {
	challengeAccepted = true;
}

void CombatInstance::endCombat(string message) {
	if (keepFighting) {
		keepFighting = false;
		//pthread_join(combatThread, NULL);
		CombatInstance::removePlayersFromCombat(playerOneID, PLAYER_ONLY);
		CombatInstance::removePlayersFromCombat(playerTwoID, enemyType);
		Server::sendMessageToCharacter(playerOneID, GameCode::COMBAT, message);
		if (enemyType == PLAYER_ONLY) {
			Server::sendMessageToCharacter(playerTwoID, GameCode::COMBAT, message);
		}
	}
	readyForCleanup = true;
}

CombatInstance::CombatInstance(int playerID, int enemyID, int givenEnemyType, int zoneID) {
	DatabaseTool::setCombatFlag(playerID, true, Target::character);

	combatZoneID = zoneID;
	enemyType = givenEnemyType;
	playerOneID = playerID;
	playerTwoID = enemyID;


	combatThread = std::thread(&CombatInstance::runCombat, this);
	combatThread.detach();
	/*
	int errorStartingThread = pthread_create(&combatThread, NULL, runCombat, NULL);
	if (errorStartingThread) {
		cout << "Failed to create combat thread: " << errorStartingThread << "\n";
		exit(-1);
	}

	pthread_exit(NULL);
	*/
}

CombatInstance::~CombatInstance() {
	if (!readyForCleanup) {
		endCombat("Your fight has been ended prematurely.\n");
	}
}