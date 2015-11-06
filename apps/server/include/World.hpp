#ifndef WORLD_HPP
#define WORLD_HPP

#include "WorldConstants.hpp"
#include "DatabaseTool.hpp"
#include "Command.hpp"
#include "Zone.hpp"
#include "Combat.hpp"
#include <boost/algorithm/string.hpp>

using namespace std;

class World {
	
	static string movePlayer(int, string);

	static string playerLook(int, string);

	static string playerPickupItem(int playerID, string item);

public:

	static string executeCommand(int, Command);
};

#endif