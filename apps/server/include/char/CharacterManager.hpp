#ifndef CHARACTERMANAGER_HPP
#define CHARACTERMANAGER_HPP


#include <string>

#include "User.hpp"


namespace CharacterManager {
	
	bool selectCharacter( User& user, std::string name, std::string sessionIdString );
	bool deselectCurrentCharacter( User& user );
	
	std::string getCharacterList( int userId );
	std::string deleteCharacter( int userId, std::string charName );
	
}


#endif