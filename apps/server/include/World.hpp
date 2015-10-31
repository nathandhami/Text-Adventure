#ifndef WORLD_HPP
#define WORLD_HPP

#include "WorldConstants.hpp"
#include "DatabaseTool.hpp"
#include "Command.hpp"
#include "Zone.hpp"
#include <boost/algorithm/string.hpp>

using namespace std;

class World {
	
	static string World::movePlayer(int, string);

	static string World::playerLook(int, string);

	static string World::playerPickupItem(int playerID, string item);

public:

	static string World::executeCommand(int, Command);
};

#endif