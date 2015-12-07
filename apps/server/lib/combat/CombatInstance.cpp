#include "CombatInstance.hpp"

// --------Private variables--------




// --------Private functions--------

void CombatInstance::removePlayerFromCombat(int playerID, Target playerType, string message = "") {
	if (playerType == Target::character) {
		DatabaseTool::setCombatFlag(playerID, false, Target::character);
		Server::sendMessageToCharacter(playerID, GameCode::COMBAT, message);
	}
	else if (playerType == Target::npc) {
		DatabaseTool::setCombatFlag(playerID, false, Target::npc);
	}
}

void CombatInstance::removePlayersFromCombat(string message = "") {
	CombatInstance::removePlayerFromCombat(playerOneID, playerOneTarget, message);
	if (playerTwoPresent) {
		CombatInstance::removePlayerFromCombat(playerTwoID, playerTwoTarget, message);
		playerTwoPresent = false;
	}
}

void CombatInstance::pushPlayerAction(int player, int action) {
	if (action == RETREAT_ACTION) {
		playersActionQueue.at(player).push_front(action);
	}
	else {
		playersActionQueue.at(player).push_back(action);
	}	
}

void CombatInstance::waitForChallengeAccept() {
	string challengerName = DatabaseTool::getCharNameFromID(playerOneID);
	string challengeeName = DatabaseTool::getCharNameFromID(playerTwoID);
	Server::sendMessageToCharacter(playerTwoID, GameCode::COMBAT, challengerName + " has challenged you to a fight!\nTo respond to their challenge, use one of the following commands\n    accept challenge " + challengerName + "\n    reject challenge " + challengerName);
	int timeoutCounter = 0;
	string helpText = "To retract your challenge use the following command\n    retract challenge " + challengeeName;

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
		Server::sendMessageToCharacter(playerOneID, GameCode::COMBAT, challengeeName + " accepted your challenge!\nYou each bow respectfully and combat begins!");
		Server::sendMessageToCharacter(playerTwoID, GameCode::COMBAT, "You each bow respectfully and combat begins!\n");
		Zone::broadcastMessage(DatabaseTool::getCharsLocation(playerOneID), challengerName + " and " + challengeeName + " have started fighting each other!", vector<int>(playerOneID, playerTwoID));
	}
	else if (keepFighting) {
		keepFighting = false;
		Server::sendMessageToCharacter(playerOneID, GameCode::COMBAT, challengeeName + " did not accept your challenge.");
		Server::sendMessageToCharacter(playerTwoID, GameCode::COMBAT, challengerName + " has given up waiting for you to respond to their challenge.");
	}
}

void CombatInstance::notifyAttack(int player, Target characterType, int damageDealt, int healthRemaining) {
	if (player == PLAYER_ONE) {
		Server::sendMessageToCharacter(playerOneID, GameCode::COMBAT, "You deal " + std::to_string(damageDealt) + " damage.");
		if (playerTwoTarget == Target::character) {
			Server::sendMessageToCharacter(playerTwoID, GameCode::COMBAT, "You take " + std::to_string(damageDealt) + " damage. You have " + std::to_string(healthRemaining) + "hp.");
		}
	}
	else {
		Server::sendMessageToCharacter(playerOneID, GameCode::COMBAT, "You take " + std::to_string(damageDealt) + " damage. You have " + std::to_string(healthRemaining) + "hp.");
		if (playerTwoTarget == Target::character) {
			Server::sendMessageToCharacter(playerTwoID, GameCode::COMBAT, "You deal " + std::to_string(damageDealt) + " damage.");
		}
	}
}

void CombatInstance::playerWin(int playerID) {
	keepFighting = false;
	if ((playerID != playerTwoID) || (playerTwoTarget != Target::npc)) {
		Attributes winGainAttributes;
		winGainAttributes.id = playerID;
		winGainAttributes.experience = 5;
		DatabaseTool::updateAttributes(winGainAttributes, Target::character);
		Character::updateStats(playerID);
		Server::sendMessageToCharacter(playerID, GameCode::COMBAT, VICTORY_NOTIFICATION);
	}
}

void CombatInstance::playerLose(int playerID) {
	keepFighting = false;
	if ((playerID == playerTwoID) && (playerTwoTarget == Target::npc)) {
		DatabaseTool::murderNpc(playerID);
	}
	else {
		Server::sendMessageToCharacter(playerID, GameCode::COMBAT, DEFEAT_NOTIFICATION);
		Zone::respawnPlayer(playerID);
	}
}

// Waiting on equipment
void CombatInstance::executePlayerAttack(int attacker, Target characterType) {
	Attributes attackerAttributes;

	Target defenderTarget;
	int defenderID = 0;
	
	if (attacker == PLAYER_ONE) {
		attackerAttributes = DatabaseTool::getAttributes(playerOneID, characterType);
		defenderTarget = playerTwoTarget;
		defenderID = playerTwoID;
	}
	else {
		attackerAttributes = DatabaseTool::getAttributes(playerTwoID, characterType);
		defenderTarget = Target::character;
		defenderID = playerOneID;
	}
	int damageDealt = attackerAttributes.strength * 2;    // Until we have weapons and armour we'll just do this for damage

	Attributes defenderAttributesModifier;
	defenderAttributesModifier.id = defenderID;
	defenderAttributesModifier.health = -damageDealt;  
	DatabaseTool::updateAttributes(defenderAttributesModifier, defenderTarget);
	Character::updateStats(defenderID);

	int healthRemaining = DatabaseTool::getAttributes(defenderID, defenderTarget).health;
	notifyAttack(attacker, characterType, damageDealt, healthRemaining);

	if (healthRemaining <= 0) {
		playerWin(attackerAttributes.id);
		playerLose(defenderID);
	}
}

void CombatInstance::executePlayerCastSpell(int player) {
	Attributes caster;
	Attributes target;
	Target targetType;
	if (player == PLAYER_ONE) {
		caster = DatabaseTool::getAttributes(player, playerOneTarget);
		targetType = playerTwoTarget;
		target = DatabaseTool::getAttributes(playerTwoID, playerTwoTarget);
	}
	else {
		caster = DatabaseTool::getAttributes(player, playerTwoTarget);
		targetType = playerTwoTarget;
		target = DatabaseTool::getAttributes(playerOneID, playerOneTarget);
	}
	Spell currentSpell = DatabaseTool::getSpell(playersSpellQueue.at(player).front());
	playersSpellQueue.at(player).pop_front();
	CastSpell::immediatelyCastSpell(&currentSpell, &caster, &target, targetType);
}

void CombatInstance::executePlayerRetreat(int player) {
	if (player == PLAYER_ONE) {
		prioritize = PLAYER_TWO;
		if (playerTwoTarget == Target::character) {
			if (DatabaseTool::getAttributes(player, playerOneTarget).health > 0) {
				Server::sendMessageToCharacter(playerOneID, GameCode::COMBAT, "You turn to run away.");
				Server::sendMessageToCharacter(playerTwoID, GameCode::COMBAT, RETREAT_NOTIFICATION);
			}
		}
	}
	else if (player == PLAYER_TWO) {
		prioritize = PLAYER_ONE;
		if (DatabaseTool::getAttributes(player, playerTwoTarget).health > 0) {
			Server::sendMessageToCharacter(playerTwoID, GameCode::COMBAT, "You turn to run away.");
			Server::sendMessageToCharacter(playerOneID, GameCode::COMBAT, RETREAT_NOTIFICATION);
		}
	}
	playersActionQueue.at(player).push_front(LEAVE_ACTION);
}

void CombatInstance::executePlayerLeave(int player) {
	keepFighting = false;
	if (player == PLAYER_ONE && playerTwoTarget == Target::character) {
		if (DatabaseTool::getAttributes(player, playerOneTarget).health > 0) {
			Server::sendMessageToCharacter(playerOneID, GameCode::COMBAT, "You get away!");
			Server::sendMessageToCharacter(playerTwoID, GameCode::COMBAT, LEAVE_NOTIFICATION);
		}
	}
	else if (player == PLAYER_TWO) {
		if (DatabaseTool::getAttributes(player, playerTwoTarget).health > 0) {
			Server::sendMessageToCharacter(playerTwoID, GameCode::COMBAT, "You get away!");
			Server::sendMessageToCharacter(playerOneID, GameCode::COMBAT, LEAVE_NOTIFICATION);
		}
	}
}

void CombatInstance::executePlayerAction(int player, Target characterType) {
	if (playersActionQueue.at(player).empty()) {
		playersActionQueue.at(player).push_back(ATTACK_ACTION);
	}
	int action = playersActionQueue.at(player).front();
	playersActionQueue.at(player).pop_front();

	if (action == ATTACK_ACTION) {
		CombatInstance::executePlayerAttack(player, characterType);
	}
	else if (action == CAST_SPELL_ACTION) {
		CombatInstance::executePlayerCastSpell(player);
	}
	else if (action == RETREAT_ACTION) {
		CombatInstance::executePlayerRetreat(player);
	}
	else if (action == LEAVE_ACTION) {
		CombatInstance::executePlayerLeave(player);
	}
}

void CombatInstance::runCombat() {
	if (playerTwoTarget == Target::character) {
		CombatInstance::waitForChallengeAccept();
	}
	else {
		DatabaseTool::setCombatFlag(playerTwoID, true, Target::npc);
		playerTwoPresent = true;
		challengeAccepted = true;   // Unnecessary, but it would annoy me if we were fighting with challengeAccepted being false =P
		string challengerName = DatabaseTool::getCharNameFromID(playerOneID);
		string npcName = DatabaseTool::getNpcName(playerTwoID);
		Zone::broadcastMessage(DatabaseTool::getCharsLocation(playerOneID), challengerName + " has started fighting " + npcName + "!", vector<int>(playerOneID));
	}
	playersSpellQueue.push_back(deque<string> ());
	playersSpellQueue.push_back(deque<string> ());
	playersActionQueue.push_back(deque<int> ());
	playersActionQueue.push_back(deque<int> ());

	while (keepFighting) {
		if (prioritize == PLAYER_TWO) {
			CombatInstance::executePlayerAction(PLAYER_TWO, playerTwoTarget);	
			CombatInstance::executePlayerAction(PLAYER_ONE, playerOneTarget);
		}
		else {
			CombatInstance::executePlayerAction(PLAYER_ONE, playerOneTarget);
			CombatInstance::executePlayerAction(PLAYER_TWO, playerTwoTarget);	
		}
		sleep(HEARTBEAT_SECONDS);
	}
}

// --------Public functions--------

bool CombatInstance::isCombatant(int playerID) {
	if (((playerOneTarget == Target::character) && (playerOneID == playerID)) || ((playerTwoTarget == Target::character) && (playerTwoID == playerID))) {
		return true;
	}
	return false;
}

bool CombatInstance::isInitiator(int playerID) {
	return (playerID == playerOneID);
}

void CombatInstance::getOpponent(int playerID, int *opponentID, Target *opponentType) {
	if (playerID == playerOneID) {
		*opponentID = playerTwoID;
		*opponentType = playerTwoTarget;
	}
	else {
		*opponentID = playerOneID;
		*opponentType = playerOneTarget;
	}
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

string CombatInstance::queuePlayerAction(int playerID, int action) {
	if (playerID == playerOneID) {
		if (playersActionQueue.at(PLAYER_ONE).size() >= CombatInstance::MAX_ACTIONS_QUEUE) {
			return "You cannot queue more than " + std::to_string(CombatInstance::MAX_ACTIONS_QUEUE) + " actions at once.";
		}
		pushPlayerAction(PLAYER_ONE, action);
	}
	else if (playerID == playerTwoID) {
		if (playersActionQueue.at(PLAYER_TWO).size() >= CombatInstance::MAX_ACTIONS_QUEUE) {
			return "You cannot queue more than " + std::to_string(CombatInstance::MAX_ACTIONS_QUEUE) + " actions at once.";
		}
		pushPlayerAction(PLAYER_TWO, action);
	}
	return "";
}

string CombatInstance::queuePlayerAction(int playerID, int action, Spell *currentSpell) {
	if (playerID == playerOneID) {
		playersSpellQueue.at(PLAYER_ONE).push_back(currentSpell->spellName);
	}
	else if (playerID == playerTwoID) {
		playersSpellQueue.at(PLAYER_TWO).push_back(currentSpell->spellName);
	}
	CombatInstance::queuePlayerAction(playerID, action);
}

void CombatInstance::acceptChallenge() {
	challengeAccepted = true;
}

void CombatInstance::rejectChallenge() {
	if (!challengeAccepted) {
		Server::sendMessageToCharacter(playerTwoID, GameCode::COMBAT, "You calmly explain to " + DatabaseTool::getCharNameFromID(playerOneID) + " that you do not want to fight at the moment.");
		Server::sendMessageToCharacter(playerOneID, GameCode::COMBAT, DatabaseTool::getCharNameFromID(playerTwoID) + " tells you you're not worthy to fight them.");
		CombatInstance::endCombat("");
	}
}

void CombatInstance::retractChallenge() {
	if (!challengeAccepted) {
		Server::sendMessageToCharacter(playerOneID, GameCode::COMBAT, "You decide you don't really want to fight " + DatabaseTool::getCharNameFromID(playerTwoID) + ", so you retract the challenge.");
		Server::sendMessageToCharacter(playerTwoID, GameCode::COMBAT, DatabaseTool::getCharNameFromID(playerOneID) + " retracts their challenge.");
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
		Server::sendMessageToCharacter(opponentID, GameCode::COMBAT, DatabaseTool::getCharNameFromID(playerID) + " disconnected, so do not worry about responding to their challenge.");
		CombatInstance::endCombat("");
	}
	else if (keepFighting) {
		Server::sendMessageToCharacter(opponentID, GameCode::COMBAT, DatabaseTool::getCharNameFromID(playerID) + " disconnected.");
		playerWin(opponentID);
		CombatInstance::endCombat("");	
	}
}

CombatInstance::CombatInstance(int playerID, int enemyID, Target givenEnemyType, int zoneID) {
	DatabaseTool::setCombatFlag(playerID, true, Target::character);

	combatZoneID = zoneID;
	playerTwoTarget = givenEnemyType;
	playerOneID = playerID;
	playerTwoID = enemyID;

	combatThread = std::thread(&CombatInstance::runCombat, this);
}

CombatInstance::~CombatInstance() {
	if (!readyForCleanup) {
		endCombat("Your fight has been ended prematurely.");
	}
}