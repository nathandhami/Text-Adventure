#ifndef COMBAT_HPP
#define COMBAT_HPP

#include "CombatInstance.hpp"
#include "CombatConstants.hpp"
#include "DatabaseTool.hpp"
#include "Server.hpp"
#include "Command.hpp"
#include <deque>
#include <boost/algorithm/string.hpp>
#include <thread>
#include <mutex>


using namespace std;

class Combat {

	static vector<std::shared_ptr<CombatInstance>> combatInstances;
	static bool cleanupThreadRunning;
	static thread cleanupThread;
	static mutex combatInstancesLock;

	static void cleanupCombats();
	static void startCombatThread();

	static std::shared_ptr<CombatInstance> getCombatInstance(int playerID, int opponentID);
	static std::shared_ptr<CombatInstance> getCombatInstance(int playerID);

	static void isolateCombatInstance(int playerID, int opponentID);

	static string startCombat(int playerID, string arguments);

	static string queueAttack(int playerID);

	static string retreat(int playerID);

	static string respondToChallenge(int playerID, string command, string arguments);

	static bool isInCombat(int characterID, int characterType);

public:

	static string executeCommand(int playerID, Command givenCommand);
	
	static bool isInCombat(int playerID);

	static void endCombat(int playerID, string message);
	static void endCombat(int playerID);

	static void endAllCombat(string message);
	static void endAllCombat();
	
	static void endAllCombat(int zoneID, string message);
	static void endAllCombat(int zoneID);

	static void playerDisconnect(int playerID);
};

#endif