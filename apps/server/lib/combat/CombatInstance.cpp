#include "CombatInstance.hpp"


// --------Private functions--------

void notifyAttack(int player, int characterType, int damageDealt, int healthRemaining) {
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

void executePlayerAttack(int player, int characterType) {
	int damageDealt = 0;
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
}

void executePlayerAction(int player, int characterType) {
	if (playersActionQueue.at(player).front() == ATTACK_ACTION) {
		executePlayerAttack(player, characterType);
	}
	else if (playersActionQueue.at(player).front() == RETREAT_ACTION) {
		keepFighting = false;
		if (player == PLAYER_ONE && enemyType == PLAYER_ONLY) {
			Server::sendMessageToCharacter(playerTwoID, GameCode::Combat, RETREAT_NOTIFICATION);
		}
		else {
			Server::sendMessageToCharacter(playerOneID, GameCode::Combat, RETREAT_NOTIFICATION);
		}
	}

	if (playersActionQueue.at(player).size() > 1) {
		playersActionQueue.at(player).pop_front();
	}
}

void removePlayersFromCombat(int playerID, int playerType) {
	if (playerType == PLAYER_ONLY) {
		// Set playerID inCombat to false
	}
}

void removePlayersFromCombat() {
	removePlayersFromCombat(playerOneID, PLAYER_ONLY);
	removePlayersFromCombat(playerTwoID, enemyType);
}

void *runCombat() {
	if (enemyType == PLAYER_ONLY) {
		// Let's be polite and ask them if they want to fight
		string challengerName = DatabaseTool::getCharName(playerOneID);
		Server::sendMessageToCharacter(playerTwoID, GameCode::Combat, challengerName + " has challenged you to a fight!\nTo accept their challenge, use the following command\n    acceptChallenge " + challengerName + "\n");
		int timeoutCounter = 0;
		while (!challengeAccepted && timeoutCounter < CHALLENGE_TIMEOUT) {
			if ((timeoutCounter % 5) == 0) {
				Server::sendMessageToCharacter(playerOneID, GameCode::Combat, "Waiting for opponent to accept challenge...\n");
			}
			sleep(HEARTBEAT_SECONDS);
			timeoutCounter++;
		}
		if (challengeAccepted) {
			// Set playerTwo inCombat
			Server::sendMessageToCharacter(playerOneID, GameCode::Combat, "Your challenge was accepted!\nYou each bow respectfully and combat begins!\n");
			Server::sendMessageToCharacter(playerTwoID, GameCode::Combat, "You each bow respectfully and combat begins!\n");
		}
		else {
			Server::sendMessageToCharacter(playerOneID, GameCode::Combat, "Your opponent did not accept your challenge.\n");
			removePlayersFromCombat(playerOneID);
			keepFighting = false;
			readyForCleanup = true;
			pthread_exit(NULL);
		}
	}
	else {
		challengeAccepted = true; // Unnecessary, but it would annoy me if we were fighting with challengeAccepted being false =P
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

void acceptChallenge() {
	challengeAccepted = true;
}

void endCombat(string message) {
	if (keepFighting) {
		keepFighting = false;
		pthread_join(combatThread);
		Server::sendMessageToCharacter(playerOneID, GameCode::Combat, message);
		if (enemyType == PLAYER_ONLY) {
			Server::sendMessageToCharacter(playerTwoID, GameCode::Combat, message);
		}
	}
	readyForCleanup = true;
}

CombatInstance(int playerID, int enemyID, int givenEnemyType) {
	// Set playerOne inCombat to true

	enemyType = givenEnemyType;
	playerOneID = playerID;
	playerTwoID = enemyID;

	int errorStartingThread = pthread_create(&combatThread, NULL, runCombat, NULL);

	pthread_exit(NULL);
}

~CombatInstance() {
	if (!readyForCleanup) {
		endCombat("Your fight has been ended prematurely.\n");
	}
}