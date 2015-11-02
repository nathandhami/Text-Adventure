#ifndef WORLDEDITOR_HPP
#define WORLDEDITOR_HPP


#include <string>


class WorldEditor {
public:
	
	static void createZone( std::string zoneData );
	static void describeZone( int creatorId, std::string zoneData );
	static void addDoorToZone( int creatorId, std::string doorData );
	
	
private:
	
	WorldEditor() {}
	
};


#endif