#ifndef COMBAT_INSTANCE_HPP
#define COMBAT_INSTANCE_HPP

#include "CombatConstants.hpp"
#include "DatabaseTool.hpp"
#include "Server.hpp"
#include "Zone.hpp"
#include "char/Character.hpp"
#include "CastSpell.hpp"
#include <deque>
#include <unistd.h>
#include <thread>
#include <limits>

#define RETREAT_NOTIFICATION "Your opponent turns to run away!"
#define LEAVE_NOTIFICATION "Your opponent got away!"
#define VICTORY_NOTIFICATION "You defeated your opponent!"
#define DEFEAT_NOTIFICATION "You were defeated!"


using namespace std;

class CombatInstance {
	static const int HEARTBEAT_SECONDS = 1;
	static const int CHALLENGE_TIMEOUT = 30;
	static const int MAX_ACTIONS_QUEUE = 10;
	// --- Do not change these ---
	static const int PLAYER_ONE = 0;
	static const int PLAYER_TWO = 1;
	// ---------------------------

	bool challengeAccepted = false;
	bool playerTwoPresent = false;
	bool keepFighting = true;
	bool endingCombat = false;
	bool readyForCleanup = false;

	int combatZoneID = 0;
	
	int playerOneID = 0;
	Target playerOneTarget = Target::character;
	int playerTwoID = 0;
	Target playerTwoTarget = Target::null;

	int prioritize = PLAYER_ONE;

	thread combatThread;

	deque<deque<int>> playersActionQueue;
	deque<deque<string>> playersSpellQueue;

	void removePlayerFromCombat(int playerID, Target playerType, string message);
	void removePlayersFromCombat(string message);

	void pushPlayerAction(int player, int action);

	void waitForChallengeAccept();

	void notifyAttack(int player, Target characterType, int damageDealt, int healthRemaining);
	
	void playerWin(int playerID);
	void playerLose(int playerID);

	void executePlayerAttack(int attacker, Target characterType);
	void executePlayerCastSpell(int player);
	
	void executePlayerRetreat(int player);
	void executePlayerLeave(int player);

	void executePlayerAction(int player, Target characterType);

	void runCombat();


public:

	bool isCombatant(int playerID);
	bool isInitiator(int playerID);
	void getOpponent(int playerID, int *opponentID, Target *opponentType);

	bool inZone(int zoneID);

	bool isStillFighting();

	bool isReadyForCleanup();

	string queuePlayerAction(int playerID, int action);
	string queuePlayerAction(int playerID, int action, Spell *currentSpell);

	void acceptChallenge();
	void rejectChallenge();
	void retractChallenge();

	void endCombat(string message);

	void playerDisconnect(int playerID);
	
	CombatInstance(int playerID, int enemyID, Target givenEnemyType, int zoneID);

	~CombatInstance();

};

#endif