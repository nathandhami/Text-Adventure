#ifndef WORLD_HPP
#define WORLD_HPP

#include "WorldConstants.hpp"
#include "DatabaseTool.hpp"
#include "Zone.hpp"
#include <boost/algorithm/string.hpp>

using namespace std;

class World {
	Zone *currentZone;

	bool movePlayer(int, Direction);
	bool playerLook(int, string);

public:

	bool executeCommand(string);

	World();
	~World();
};

#endif