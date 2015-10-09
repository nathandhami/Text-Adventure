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

	void setID(int);
	int getID();

	void setName();
	string getName();

	void setDescription();
	string getDescription(string);

	void setNeighbourZone(Direction, int);
	int getNeighbourZone(Direction);

	bool roomForMorePlayers();

	void playerEnteringZone(int);

	Zone(int);
	~Zone();
};

#endif