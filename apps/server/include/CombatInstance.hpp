#ifndef COMBAT_INSTANCE_HPP
#define COMBAT_INSTANCE_HPP

#include "CombatConstants.hpp"
#include "DatabaseTool.hpp"
#include "Server.hpp"
#include "Zone.hpp"
#include <deque>
#include <unistd.h>
#include <pthread.h>

using namespace std;

class CombatInstance {
	static const int PLAYER_OR_NPC = 0;
	static const int PLAYER_ONLY = 1;
	static const int NPC_ONLY = 2;
	static const int HEARTBEAT_SECONDS = 1;
	static const int CHALLENGE_TIMEOUT = 30;
	static const int PLAYER_ONE = 0;
	static const int PLAYER_TWO = 1;
	static const int ATTACK_ACTION = 0;
	static const int RETREAT_ACTION = 1;
	static const string RETREAT_NOTIFICATION = "Your opponent turns to flee.";
	static const string VICTORY_NOTIFICATION = "You defeated your opponent!\n";
	static const string DEFEAT_NOTIFICATION = "You were defeated!\n";

	bool challengeAccepted = false;
	bool keepFighting = true;
	bool readyForCleanup = false;

	int combatZoneID = 0;
	int enemytype = 0;
	int playerOneID = 0;
	int playerTwoID = 0;

	pthread_t combatThread;

	deque<deque<int>> playersActionQueue;

	void CombatInstance::notifyAttack(int player, int characterType, int damageDealt, int healthRemaining);
	
	void CombatInstance::executePlayerAttack(int player, int characterType);
	
	void CombatInstance::executePlayerRetreat(int player);

	void CombatInstance::executePlayerAction(int player, int characterType);

	void CombatInstance::removePlayersFromCombat(int playerID, int playerType);
	void CombatInstance::removePlayersFromCombat();

	void *CombatInstance::runCombat();


public:

	bool CombatInstance::isCombatant(int playerID);

	bool CombatInstance::inZone(int zoneID);

	void CombatInstance::acceptChallenge();

	void CombatInstance::endCombat(string message);
	
	CombatInstance::CombatInstance(int playerID, int enemyID, int givenEnemyType, int zoneID);

	CombatInstance::~CombatInstance();

};

#endif