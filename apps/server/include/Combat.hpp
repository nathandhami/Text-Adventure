#ifndef COMBAT_HPP
#define COMBAT_HPP

#include "CombatInstance.hpp"
#include "CombatConstants.hpp"
#include "DatabaseTool.hpp"
#include "Server.hpp"
#include <deque>
#include <boost/algorithm/string.hpp>

using namespace std;

class CombatActions {
	static const string HOW_TO_START_FIGHT = "To start a fight, use one of these commands\n    fight <name>\n    fight player <name>\n    fight npc <name>\n";
	
	static deque<CombatInstance> combatInstances;


	static CombatInstance *Combat::getCombatInstance(int playerID);

	static string Combat::startCombat(int playerID, string arguments);

	static string Combat::queueAttack(int playerID);

	static string Combat::retreat(int playerID);

	static string Combat::acceptChallenge(int playerID, string arguments);

	static bool Combat::isInCombat(int characterID, int characterType);

public:

	static string Combat::executeCommand(int playerID, Command givenCommand);
	
	static bool Combat::isInCombat(int playerID);

	static void Combat::endCombat(int playerID, string message);
	static void Combat::endCombat(int playerID);

	static void Combat::endAllCombat(string message);
	static void Combat::endAllCombat();
	
	static void Combat::endAllCombat(int zoneID, string message);
	static void Combat::endAllCombat(int zoneID);
};

#endif