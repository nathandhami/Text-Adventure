#ifndef CHARACTER_HPP
#define CHARACTER_HPP


#include <string>
#include <utility>

#include "Command.hpp"


class Character {
	
public:
	static std::pair< std::string, std::string > performCommand( int charId, Command command );
	static std::string getStats( int charId );
	static void updateStats( int charId );
	
	
private:
	Character() {}
	
	static std::string look( int charId, std::string direction );
	static std::string lookAt( int charId, std::string keyword );
	
	static std::string getInventoryContents( int charId );
	static void updateInventory( int charId );
};


#endif