#ifndef CHARACTERMANAGER_HPP
#define CHARACTERMANAGER_HPP


#include <string>
#include <utility>

#include "User.hpp"


class CharacterManager {

public:	
	static bool selectCharacter( User& user, std::string name, std::string sessionIdString );
	static bool deselectCurrentCharacter( User& user );

	static std::string getCharacterList( int userId );
	static std::pair< std::string, std::string > createCharacter( int userId, std::string charName );
	static std::string deleteCharacter( int userId, std::string charName );
	
private:
	CharacterManager() {}
	
};


#endif