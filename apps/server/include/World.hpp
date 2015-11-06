#ifndef WORLD_HPP
#define WORLD_HPP

#include "WorldConstants.hpp"
#include "DatabaseTool.hpp"
#include "Command.hpp"
#include "Zone.hpp"
#include "Combat.hpp"
#include <boost/algorithm/string.hpp>
#include <thread>

using namespace std;

class World {
	static const int RESPAWN_TIME_SECONDS = 1800;
	
	static bool keepRespawning;

	static std::thread respawnThread;


	static string movePlayer(int, string);
	
	static string playerLook(int, string);

	static string playerPickupItem(int playerID, string item);

	static void runRespawn();

public:

	static string executeCommand(int, Command);

	static bool isRespawnLoopRunning();

	static void startRespawnLoop();
	static void stopRespawnLoop();
	static void respawnImmediately();
};

#endif