#ifndef WORLD_HPP
#define WORLD_HPP

#include "WorldConstants.hpp"
#include "DatabaseTool.hpp"
#include "CommandParser.hpp"
#include "Zone.hpp"
#include <boost/algorithm/string.hpp>

using namespace std;

class World {
	static Zone* currentZone;

	bool movePlayer(int, string);
	string playerLook(int, string);

public:

	string executeCommand(Command*);

	World();
	~World();
};

#endif