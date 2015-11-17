#ifndef WORLDEDITOR_HPP
#define WORLDEDITOR_HPP


#include <string>


class WorldEditor {
public:
	
	static std::string createZone( std::string zoneData );
	static std::string deleteZone( std::string zoneData );
	
	static std::string describeZone( int creatorId, std::string zoneData );
	
	static std::string addDoorToZone( int creatorId, std::string doorData );
	static std::string deleteDoor( int creatorId, std::string doorData );
	
	
private:
	
	WorldEditor() {}
	
};


#endif