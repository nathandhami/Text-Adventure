#ifndef WORLD_HPP
#define WORLD_HPP

#include "WorldConstants.hpp"
#include "DatabaseTool.hpp"
#include "Command.hpp"
#include "char/Character.hpp"
#include "Zone.hpp"
#include "Combat.hpp"
#include <boost/algorithm/string.hpp>
#include <thread>

using namespace std;

class World {
	static const int RESPAWN_TIME_SECONDS = 900;
	static const int REGEN_TIME_SECONDS = 10;
	static const int HEALTH_REGEN_AMOUNT = 1;
	static const int MANA_REGEN_AMOUNT = 2;
	
	static bool keepRespawning;

	static std::thread respawnThread;


	static string movePlayer(int, string);
	
	static string playerLook(int, string);
	static string playerLookAt(int, string);

	static string playerPickupItem(int playerID, string item);
	static string playerDropItem(int playerID, string item);

	static void regenHealthAndMana();
	static void runRespawn();

public:

	static string executeCommand(int, Command);

	static bool isRespawnLoopRunning();

	static void startRespawnLoop();
	static void stopRespawnLoop();
	static void respawnImmediately();
};

#endif