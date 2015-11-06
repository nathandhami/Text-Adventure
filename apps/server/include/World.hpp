#ifndef WORLD_HPP
#define WORLD_HPP

#include "WorldConstants.hpp"
#include "DatabaseTool.hpp"
#include "Command.hpp"
#include "Zone.hpp"
#include <boost/algorithm/string.hpp>
#include <thread>

using namespace std;

class World {
	static const int RESPAWN_TIME_SECONDS = 1800;
	
	bool keepRespawning = false;

	std::thread respawnThread;

	static bool movePlayer(int, string);
	
	static string playerLook(int, string);

public:

	static string executeCommand(int, Command);
};

#endif