#ifndef ZONE_HPP
#define ZONE_HPP

#include "WorldConstants.hpp"

using namespace std;

class Zone {
	const int MAX_PLAYERS = 5;   // A number to be determined, maybe even dynamic
	int zoneID;

	vector<int> getNeighbourZones();
	vector<int> getPlayerIDs();
	vector<int> getNpcIDs();
	vector<int> getObjectIDs();

public:

	void setID(int id);
	int getID();

	void setName();
	string getName();

	void setDescription();
	string getDescription();

	void setNeighbourZone(int direction, int neighbourID);
	int getNeighbourZone(int direction);

	bool roomForMorePlayers();

	void playerEnteringZone(int playerID);

	Zone(int givenZoneID);
	~Zone();
};

#endif