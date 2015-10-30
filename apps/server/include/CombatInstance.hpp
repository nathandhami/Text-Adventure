#ifndef COMBAT_INSTANCE_HPP
#define COMBAT_INSTANCE_HPP

#include "CombatConstants.hpp"
#include "DatabaseTool.hpp"
#include "Server.hpp"
#include <deque>
#include <unistd.h>
#include <pthread.h>

using namespace std;

class CombatInstance {
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

	int enemytype = 0;
	int playerOneID = 0;
	int playerTwoID = 0;

	pthread_t combatThread;

	deque<deque<int>> playersActionQueue;

	static void CombatInstance::notifyAttack(int player, int characterType, int damageDealt, int healthRemaining);
	
	static void CombatInstance::executePlayerAttack(int player, int characterType);
	
	static void CombatInstance::executePlayerRetreat(int player);
	
	static void CombatInstance::executePlayerAction(int player, int characterType);

	static void CombatInstance::removePlayersFromCombat(int playerID, int playerType);
	static void CombatInstance::removePlayersFromCombat();

	static void *CombatInstance::runCombat();


public:

	static void CombatInstance::acceptChallenge();

	static void CombatInstance::endCombat(string message);
	
	static CombatInstance::CombatInstance(int playerID, int enemyID, int givenEnemyType);

	static CombatInstance::~CombatInstance();

};

#endif