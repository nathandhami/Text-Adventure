#ifndef ZONE_HPP
#define ZONE_HPP

#include "WorldConstants.hpp"
#include "DatabaseTool.hpp"
#include "Server.hpp"
#include <vector>

using namespace std;

class Zone {

	static vector<int> Zone::getNeighbourZones(int);
	static vector<int> Zone::getPlayerIDs(int);
	static vector<int> Zone::getNpcIDs(int);
	static vector<int> Zone::getObjectIDs(int);

public:

	static void Zone::setName(int, string);
	static string Zone::getName(int);

	static void Zone::setDescription(int, string);
	static string Zone::getDescription(int, string);

	static void Zone::setNeighbourZone(int, string, int);
	static int Zone::getNeighbourZone(int, string);

	static bool Zone::roomForMorePlayers(int);

	static void Zone::broadcastMessage(int zoneID, string message);
};

#endif