#ifndef COMBAT_INSTANCE_HPP
#define COMBAT_INSTANCE_HPP

#include "CombatConstants.hpp"
#include "DatabaseTool.hpp"
#include "Server.hpp"
#include "Zone.hpp"
#include <deque>
#include <unistd.h>
#include <thread>

#define RETREAT_NOTIFICATION "Your opponent turns to flee.\n"
#define VICTORY_NOTIFICATION "You defeated your opponent!\n"
#define DEFEAT_NOTIFICATION "You were defeated!\n"

using namespace std;

class CombatInstance {
	static const int HEARTBEAT_SECONDS = 1;
	static const int CHALLENGE_TIMEOUT = 30;
	static const int PLAYER_ONE = 0;
	static const int PLAYER_TWO = 1;

	static bool challengeAccepted;
	static bool keepFighting;
	static bool readyForCleanup;

	static int combatZoneID;
	static int enemyType;
	static int playerOneID;
	static int playerTwoID;

	// For testing combat until we have equipment and attribute data
	static int damageDealt;
	static int playerOneHealthRemaining;
	static int playerTwoHealthRemaining;

	static std::thread combatThread;

	static deque<deque<int>> playersActionQueue;


	void removePlayersFromCombat(int playerID, int playerType);
	void removePlayersFromCombat();

	void waitForChallengeAccept();

	void notifyAttack(int player, int characterType, int damageDealt, int healthRemaining);
	
	void executePlayerAttack(int player, int characterType);
	
	void executePlayerRetreat(int player);

	void executePlayerAction(int player, int characterType);

	void runCombat();


public:

	bool isCombatant(int playerID);

	bool inZone(int zoneID);

	void queuePlayerAction(int playerID, int action);

	void acceptChallenge();

	void endCombat(string message);
	
	CombatInstance(int playerID, int enemyID, int givenEnemyType, int zoneID);

	~CombatInstance();

};

#endif