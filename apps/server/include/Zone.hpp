#ifndef ZONE_HPP
#define ZONE_HPP

#include "WorldConstants.hpp"
#include "DatabaseTool.hpp"
#include "Server.hpp"
#include <vector>

using namespace std;

class Zone {

	static vector<int> getNeighbourZones(int);
	static vector<int> getPlayerIDs(int);
	static vector<int> getNpcIDs(int);
	static vector<int> getObjectIDs(int);

public:

	static void setName(int, string);
	static string getName(int);

	static void setDescription(int, string);

	static void setNeighbourZone(int, string, int);
	static int getNeighbourZone(int, string);

	static bool roomForMorePlayers(int);

	static void broadcastMessage(int zoneID, string message);
};

#endif