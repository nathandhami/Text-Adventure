#ifndef ZONE_HPP
#define ZONE_HPP

#include "WorldConstants.hpp"
#include "Zone.hpp"

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