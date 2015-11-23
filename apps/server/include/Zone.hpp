#ifndef ZONE_HPP
#define ZONE_HPP

#include "WorldConstants.hpp"
#include "DatabaseTool.hpp"
#include "Server.hpp"
#include <vector>

using namespace std;

class Zone {

	static vector<int> getPlayerIDs(int);
	static vector<int> getNpcIDs(int);

public:

	static string getName(int);

	static int getNeighbourZone(int, string);

	static void broadcastMessage(int zoneID, string message, vector<int> exceptPlayers);
	static void broadcastMessage(int zoneID, string message);
};

#endif