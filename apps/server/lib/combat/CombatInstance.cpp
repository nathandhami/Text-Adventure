#include "CombatInstance.hpp"

// --------Private variables--------




// --------Private functions--------

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
	cout << "WAITING FOR PLAYER_TWO TO ACCEPT CHALLENGE" << endl;
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
		playerTwoPresent = true;
		Server::sendMessageToCharacter(playerOneID, GameCode::COMBAT, challengeeName + " accepted your challenge!\nYou each bow respectfully and combat begins!\n");
		Server::sendMessageToCharacter(playerTwoID, GameCode::COMBAT, "You each bow respectfully and combat begins!\n");
		Zone::broadcastMessage(DatabaseTool::getCharsLocation(playerOneID), challengerName + " and " + challengeeName + " have started fighting each other!\n");
	}
	else if (keepFighting) {
		keepFighting = false;
		Server::sendMessageToCharacter(playerOneID, GameCode::COMBAT, challengeeName + " did not accept your challenge.\n");
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

// Waiting on equipment and attributes data
// In the meantime no actual death or saved health for players (unless NPC)
void CombatInstance::executePlayerAttack(int player, int characterType) {
	
	// Temporary, rigged just to show messaging works.
	if (player == PLAYER_ONE) {
		playerTwoHealthRemaining -= damageDealt;
		if (playerTwoHealthRemaining <= 0) {
			keepFighting = false;
			if (characterType == NPC_ONLY) {
				DatabaseTool::murderNpc(playerTwoID);
			}
		}
		notifyAttack(player, characterType, damageDealt, playerTwoHealthRemaining);
	}
	else {
		playerOneHealthRemaining -= damageDealt;
		if (playerOneHealthRemaining <= 0) {
			keepFighting = false;
		}
		notifyAttack(player, characterType, damageDealt, playerOneHealthRemaining);
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
		Server::sendMessageToCharacter(playerTwoID, GameCode::COMBAT, RETREAT_NOTIFICATION);
	}
	else if (player == PLAYER_TWO) {
		Server::sendMessageToCharacter(playerOneID, GameCode::COMBAT, RETREAT_NOTIFICATION);
	}
}

void CombatInstance::executePlayerAction(int player, int characterType) {
	cout << "CHECK 1" << endl;
	if (playersActionQueue.at(player).empty()) {
		cout << "CHECK 2" << endl;
		playersActionQueue.at(player).push_back(ATTACK_ACTION);
	}
	cout << "CHECK 3" << endl;

	if (playersActionQueue.at(player).front() == ATTACK_ACTION) {
		CombatInstance::executePlayerAttack(player, characterType);
	}
	else if (playersActionQueue.at(player).front() == RETREAT_ACTION) {
		CombatInstance::executePlayerRetreat(player);
	}
	cout << "CHECK 4" << endl;

	playersActionQueue.at(player).pop_front();
	cout << "CHECK 5" << endl;
}

void CombatInstance::runCombat() {
	cout << "RUNNING COMBAT" << endl;

	if (enemyType == PLAYER_ONLY) {
		CombatInstance::waitForChallengeAccept();
	}
	else {
		cout << "FIGHTING NPC" << endl;
		DatabaseTool::setCombatFlag(playerTwoID, true, Target::npc);
		playerTwoPresent = true;
		challengeAccepted = true;   // Unnecessary, but it would annoy me if we were fighting with challengeAccepted being false =P
		string challengerName = DatabaseTool::getCharNameFromID(playerOneID);
		string npcName = DatabaseTool::getNpcName(playerTwoID);
		Zone::broadcastMessage(DatabaseTool::getCharsLocation(playerOneID), challengerName + " has started fighting " + npcName + "!\n");
	}

	playersActionQueue.push_back(deque<int> (1, ATTACK_ACTION));
	playersActionQueue.push_back(deque<int> (1, ATTACK_ACTION));

	while (keepFighting) {
		CombatInstance::executePlayerAction(PLAYER_ONE, PLAYER_ONLY);
		CombatInstance::executePlayerAction(PLAYER_TWO, enemyType);
		sleep(HEARTBEAT_SECONDS);
	}



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

void CombatInstance::endCombat(string message) {
	cout << "ENDING COMBAT" << endl;
	keepFighting = false;
	cout << "JOINING THREAD" << endl;
	combatThread.join();
	cout << "REMOVING PLAYERS FROM COMBAT" << endl;
	CombatInstance::removePlayersFromCombat(message);
	
	readyForCleanup = true;
	cout << "READY FOR CLEANUP" << endl;
}

CombatInstance::CombatInstance(int playerID, int enemyID, int givenEnemyType, int zoneID) {
	DatabaseTool::setCombatFlag(playerID, true, Target::character);

	combatZoneID = zoneID;
	enemyType = givenEnemyType;
	playerOneID = playerID;
	playerTwoID = enemyID;


	combatThread = std::thread(&CombatInstance::runCombat, this);
	/*
	int errorStartingThread = pthread_create(&combatThread, NULL, runCombat, NULL);
	if (errorStartingThread) {
<<<<<<< HEAD
		cout << "Failed to create combat thread: " << errorStartingThread << "\n";
		exit(-1);
	}

=======
		cout << "Failed to start CombatInstance thread: " << errorStartingThread << endl;
        exit(-1);
	}
>>>>>>> 9e3c3ca78bad4f3af97b48acda0fe1d8d5efdd11
	pthread_exit(NULL);
	*/
}

CombatInstance::~CombatInstance() {
	if (!readyForCleanup) {
		endCombat("Your fight has been ended prematurely.\n");
	}
}