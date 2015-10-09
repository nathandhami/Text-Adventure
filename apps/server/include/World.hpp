#ifndef ZONE_HPP
#define ZONE_HPP

#include "WorldConstants.hpp"
#include "Zone.hpp"

using namespace std;

class World {
	Zone *currentZone;

	bool movePlayer(int, Direction);

public:

	

	World();
	~World();
};

#endif