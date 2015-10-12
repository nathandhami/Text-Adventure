#ifndef WORLD_HPP
#define WORLD_HPP

#include "WorldConstants.hpp"
#include "DatabaseTool.hpp"
#include "Command.hpp"
#include "Zone.hpp"
#include <boost/algorithm/string.hpp>

using namespace std;

class World {
	
	static bool movePlayer(int, string);
	static string playerLook(int, string);

public:

	static string executeCommand(int, Command*);
};

#endif