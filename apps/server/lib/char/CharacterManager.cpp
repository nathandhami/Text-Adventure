#include <char/CharacterManager.hpp>
#include "DatabaseTool.hpp"

#include <vector>
#include <boost/algorithm/string.hpp>


// ------------------- PUBLIC -------------------

std::string CharacterManager::getCharacterList( int userId ) {
	std::vector< int > charIds = DatabaseTool::getAllUserCharIDs( userId );
	
	std::vector< std::string > charStrings;
	
	for ( int& charId: charIds ) {
		std::string charName = DatabaseTool::getCharNameFromID( charId );
		std::string charLevel = std::to_string( DatabaseTool::getAttributes( charId, Target::character ).level );
		std::string charLocation = DatabaseTool::getZoneName( DatabaseTool::getCharsLocation( charId ) ) + ", Midgard";
		std::string charDescription = DatabaseTool::getCharDescription( charId );
		charStrings.push_back( 
			charName + "|" + charLevel + "|" + charLocation + "|" + charDescription
		);
	}
	
	return boost::algorithm::join( charStrings, "~" );
	
}


// ------------------- PRIVATE ------------------