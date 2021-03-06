#ifndef ZONE_HPP
#define ZONE_HPP

#include "WorldConstants.hpp"
#include "DatabaseTool.hpp"
#include "char/Character.hpp"
#include "Server.hpp"
#include <vector>

using namespace std;

class Zone {
	static const int RESPAWN_ZONE_ID = 3054;

	static vector<int> getPlayerIDs(int);
	static vector<int> getNpcIDs(int);

public:

	static string getName(int);

	static int getNeighbourZone(int, string);

	static void broadcastMessage(int zoneID, string message, vector<int> exceptPlayers);
	static void broadcastMessage(int zoneID, string message);

	// Technically belongs in World or its own class, but when in World causes circular dependency
	static void respawnPlayer(int playerID);
};

#endif