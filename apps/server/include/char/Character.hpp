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
	
	static std::string getInventory( int charId );
	static void updateInventory( int charId );
	
	
private:
	Character() {}
	
	static std::string look( int charId, std::string direction );
	static std::string lookAt( int charId, std::string keyword );
	
	static std::string move( int charId, std::string direction );
	
	static std::string pickUpItem( int charId, std::string keyword );
	static std::string dropItem( int charId, std::string keyword );
	static std::string equipItem( int charId, std::string keyword );
	static std::string unequipItem( int charId, std::string keyword );
	static std::string buySpell( int charId, std::string keyword );
};


#endif