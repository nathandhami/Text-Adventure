#include <char/CharacterManager.hpp>
#include "DatabaseTool.hpp"
#include <GameCode.hpp>

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


bool CharacterManager::selectCharacter( User& user, std::string name, std::string sessionIdString ) {
	int charId = DatabaseTool::getCharIDFromName( name );
	
	if ( !charId ) return false;
	if ( DatabaseTool::isCharOnline( charId ) ) return false;
	
	DatabaseTool::setCharOnline( charId, sessionIdString );
	user.selectedCharacterId = charId;
	return true;
}


void CharacterManager::deselectCurrentCharacter( User& user ) {
	DatabaseTool::setCharOffline( user.getSelectedCharacterId() );
	user.selectedCharacterId = 0;
}


std::pair< std::string, std::string > CharacterManager::createCharacter( int userId, std::string charData ) {
	const int STARTING_ZONE_ID = 3054;
	const int STARTING_ITEM_ID = 10003;
	const int NUM_ARGS_EXP = 2;
	
	std::vector< std::string > charTokens;
	boost::split( charTokens, charData, boost::is_any_of( "|" ) );
	
	if ( charTokens.size() != NUM_ARGS_EXP ) return std::make_pair( GameCode::ERROR, "Input error: missing arguments." );
	std::string charName = charTokens[ 0 ];
	std::string charDescription = charTokens[ 1 ];
	
	if ( DatabaseTool::addCharacter( charName, userId, charDescription ) ) {
		return std::make_pair( GameCode::OK, CharacterManager::getCharacterList( userId ) );
	} else {
		return std::make_pair( GameCode::ERROR, "Internal server error: unable to save character." );
	}
}


std::pair< std::string, std::string > CharacterManager::deleteCharacter( int userId, std::string charName ) {
	if ( DatabaseTool::removeCharacter( charName ) ) {
		return std::make_pair( GameCode::OK, CharacterManager::getCharacterList( userId ) );
	} else {
		return std::make_pair( GameCode::ERROR, "Internal server error: unable to delete character." );
	}
}

// ------------------- PRIVATE ------------------