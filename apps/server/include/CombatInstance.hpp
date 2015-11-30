#ifndef COMBAT_INSTANCE_HPP
#define COMBAT_INSTANCE_HPP

#include "CombatConstants.hpp"
#include "DatabaseTool.hpp"
#include "Server.hpp"
#include "Zone.hpp"
#include <deque>
#include <unistd.h>
#include <thread>

#define RETREAT_NOTIFICATION "Your opponent ran away.\n"
#define VICTORY_NOTIFICATION "You defeated your opponent!\n"
#define DEFEAT_NOTIFICATION "You were defeated!\n"


using namespace std;

class CombatInstance {
	static const int HEARTBEAT_SECONDS = 1;
	static const int CHALLENGE_TIMEOUT = 30;
	static const int PLAYER_ONE = 0;
	static const int PLAYER_TWO = 1;

	bool challengeAccepted = false;
	bool playerTwoPresent = false;
	bool keepFighting = true;
	bool endingCombat = false;
	bool readyForCleanup = false;

	int combatZoneID = 0;
	int enemyType = 0;
	int playerOneID = 0;
	int playerTwoID = 0;

	// For testing combat until we have equipment and attribute data
	int damageDealt = 4;
	int playerOneHealthRemaining = 60;
	int playerTwoHealthRemaining = 55;

	thread combatThread;

	deque<deque<int>> playersActionQueue;

	void getTargetFromType(int characterType);

	void removePlayerFromCombat(int playerID, int playerType, string message);
	void removePlayersFromCombat(string message);

	void pushPlayerAction(int player, int action);

	void waitForChallengeAccept();

	void notifyAttack(int player, int characterType, int damageDealt, int healthRemaining);
	
	void playerWin(int playerID);
	void playerLose(int playerID);

	void executePlayerAttack(int attacker, int characterType);
	
	void executePlayerRetreat(int player);

	void executePlayerAction(int player, int characterType);

	void runCombat();


public:

	bool isCombatant(int playerID);
	bool isInitiator(int playerID);

	bool inZone(int zoneID);

	bool isStillFighting();

	bool isReadyForCleanup();

	void queuePlayerAction(int playerID, int action);

	void acceptChallenge();
	void rejectChallenge();
	void retractChallenge();

	void endCombat(string message);

	void playerDisconnect(int playerID);
	
	CombatInstance(int playerID, int enemyID, int givenEnemyType, int zoneID);

	~CombatInstance();

};

#endif