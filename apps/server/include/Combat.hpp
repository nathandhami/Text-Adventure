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
	static const int PLAYER_OR_NPC = 0;
	static const int PLAYER_ONLY = 1;
	static const int NPC_ONLY = 2;
	static const string HOW_TO_START_FIGHT = "To start a fight, use one of these commands\n    fight <name>\n    fight player <name>\n    fight npc <name>\n";
	
	static list<CombatInstance> combatInstances;

	static CombatInstance Combat::getCombatInstance(int playerID);

	static deque<CombatInstance> Combat::getCombatInstances(int zoneID);

	static int Combat::getLocalIDFromName(int playerID, int characterName, int characterType);

	static string Combat::startCombat(int playerID, string arguments);

	static string Combat::queueAttack(int playerID);

	static string Combat::retreat(int playerID);

	static string Combat::acceptChallenge(int playerID, string arguments);

public:

	static string Combat::executeCommand(int playerID, Command givenCommand);

	static bool Combat::isInCombat(int characterID, int characterType);
	static bool Combat::isInCombat(int playerID);

	static void Combat::endAllCombat(string message);
	static void Combat::endAllCombat();
	static void Combat::endAllCombat(int zoneID, string message);
	static void Combat::endAllCombat(int zoneID);
};

#endif