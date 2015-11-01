#include <mod/Editor.hpp>
#include "DatabaseTool.hpp"


int main() {
	
//	Editor::judgeAndPerform( 1 );
	std::string direction = "north";
	std::cout << "Door points to zoneID: " << DatabaseTool::getZoneIDBehindDoorAt( 4, direction ) << std::endl;
	std::cout << DatabaseTool::getDoorDescription( 4, direction ) << std::endl;
	
//	DatabaseTool::moveCharacterToZone( 1, 4 );
	
	return 0;
}