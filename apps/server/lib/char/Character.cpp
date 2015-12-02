#include <char/Character.hpp>
#include "DatabaseTool.hpp"
#include <GameCode.hpp>
#include "Server.hpp"

#include <boost/algorithm/string/join.hpp>
#include <boost/algorithm/string.hpp>


// ------------------- PUBLIC -------------------

std::pair< std::string, std::string > Character::performCommand( int charId, Command command ) {
	const std::string NOT_ALLOWED = "You cannot do that!";
	const std::string CMD_LOOK		= "look";
	const std::string CMD_LOOK_AT	= "look at";
	const std::string CMD_MOVE		= "move";
	const std::string CMD_PICK_UP	= "pick up";
	const std::string CMD_DROP		= "drop";
	
	if ( command.type == CMD_LOOK ) {
		return std::make_pair( GameCode::DESCRIPTION, Character::look( charId, command.data ) );
	} else if ( command.type == CMD_LOOK_AT ) {
		return std::make_pair( GameCode::DESCRIPTION, Character::lookAt( charId, command.data ) );
	} else if ( command.type == CMD_MOVE ) {
		return std::make_pair( GameCode::STATUS, Character::move( charId, command.data ) );
	} else if ( command.type == CMD_PICK_UP ) {
		return std::make_pair( GameCode::STATUS, Character::pickUpItem( charId, command.data ) );
	} else if ( command.type == CMD_DROP ) {
		return std::make_pair( GameCode::STATUS, Character::dropItem( charId, command.data ) );
	}
	
	//TO-DO: add an 'else' for invalid action
}


std::string Character::getStats( int charId ) {
	Attributes stats = DatabaseTool::getAttributes( charId, Target::character );
	return
		"Strength:" 	+ std::to_string( stats.strength ) + "\n" +
		"Intelligence:" + std::to_string( stats.intelligence ) + "\n" +
		"Dexterity:" 	+ std::to_string( stats.dexterity ) + "\n" +
		"Charisma:" 	+ std::to_string( stats.charisma ) + "\n" +
		"HP:"			+ std::to_string( stats.health ) + "/" + std::to_string( stats.maxHealth ) + "\n" +
		"MP:"			+ std::to_string( stats.mana ) + "/" + "what" + "\n" +
		"XP:"			+ std::to_string( stats.experience ) + "/" + "what" + "\n" +
		"Level:"		+ std::to_string( stats.level );
}


void Character::updateStats( int charId ) {
	Server::sendMessageToCharacter( charId, GameCode::ATTRIBUTES, Character::getStats( charId ) );
}


std::string Character::getInventory( int charId ) {
	std::vector< Item > items = DatabaseTool::getItemsInInventory( charId );
	std::vector< std::string > formattedItems;

	for ( Item& item: items ) {
		formattedItems.push_back( item.shortDesc + ";" + std::to_string( item.quantity ) + ";" + std::to_string( item.isEquipped ) );
	}
	return boost::algorithm::join( formattedItems, "\n" );
}


void Character::updateInventory( int charId ) {
	Server::sendMessageToCharacter( charId, GameCode::INVENTORY, Character::getInventory( charId ) );
}


// ------------------- PRIVATE ------------------

std::string Character::look( int charId, std::string direction ) {
	Character::updateStats( charId );
	
	int currentZoneId = DatabaseTool::getCharsLocation( charId );
	std::string description = "";
	
	if ( direction == "" ) {
		return DatabaseTool::getZoneDesc( currentZoneId );
	} else {
		return DatabaseTool::getDirectionDesc( currentZoneId, direction );
	}
}


std::string Character::lookAt( int charId, std::string keyword ) {
	const std::string NOTHING_TO_LOOK_AT = "Quite indecisive, aren't we?";
	const std::string KW_INVENTORY	= "inventory";
	const std::string KW_OBJECTS 	= "objects";
	const std::string KW_NPCS		= "people";
	const std::string KW_PLAYERS	= "players";
	
	
	int currentZoneId = DatabaseTool::getCharsLocation( charId );
//	std::string description = "";
	
	if ( keyword == "" ) {
		return NOTHING_TO_LOOK_AT;
	} else if ( keyword == KW_OBJECTS ) {
		std::vector< std::string > objects = DatabaseTool::getItemsInZone( currentZoneId );
		if ( objects.empty() ) return "You don't see anything of particular interest.";
		return ( "You see " + boost::algorithm::join( objects, ", " ) + "." );
	} else if ( keyword == KW_NPCS ) {
		std::vector< std::string > npcs = DatabaseTool::getAllNPCsInZone( currentZoneId );
		if ( npcs.empty() ) return "You don't see any local folk.";
		return ( "You see " + boost::algorithm::join( npcs, ", " ) + "." );
	} else if ( keyword == KW_PLAYERS ) {
		std::vector< std::string > players = DatabaseTool::getAllPlayersInZone( charId, currentZoneId );
		if ( players.empty() ) return "You don't see any adventurers.";
		return ( "You see " + boost::algorithm::join( players, ", " ) + "." );
	}else {
		std::vector< std::string > kwDescriptions;
		
		std::string players = DatabaseTool::findPlayerDescription( charId, currentZoneId, keyword );
		if ( !players.empty() ) kwDescriptions.push_back( players );
		
		std::string npcs = DatabaseTool::findNpcDescription( currentZoneId, keyword );
		if ( !npcs.empty() ) kwDescriptions.push_back( npcs );
		
		std::string objects = DatabaseTool::findItemDescription( charId, currentZoneId, keyword );
		if ( !objects.empty() ) kwDescriptions.push_back( objects );
		
		if( kwDescriptions.empty() ) return ( "You don't see any " + keyword + "." );
		return ( "You see " + boost::algorithm::join( kwDescriptions, ", " ) + "." );
	}
}


std::string Character::move( int charId, std::string destination ) {
	int currentZoneId = DatabaseTool::getCharsLocation( charId );
	
	int destinationZoneId = DatabaseTool::getDirectionID( currentZoneId, destination );
	if ( !destinationZoneId ) {
		return ( "There is nothing there." );
	} else {
		DatabaseTool::putCharInZone( charId, destinationZoneId );
		return Character::look( charId, "" ); //i think it'd be to replace this with "You are in ..."
	}
}


std::string Character::pickUpItem( int charId, std::string keyword ) {
	//TO-DO: add item to inventory
	int currentZoneID = DatabaseTool::getCharsLocation( charId );
	if ( !DatabaseTool::pickUp( charId, keyword ) ) {
//		Zone::broadcastMessage(currentZoneID, DatabaseTool::getCharNameFromID(playerID) + " picked up " + item);
		return "You grabbed the air as if something was in it, but your hand remained empty.";
	}
	
	Character::updateInventory( charId );
	return ( "You picked up " + keyword );
	//return stuff
}


std::string Character::dropItem( int charId, std::string keyword ) {
	int currentZoneID = DatabaseTool::getCharsLocation( charId );
	if ( !DatabaseTool::dropItem( charId, keyword ) ) {
//		Zone::broadcastMessage(currentZoneID, DatabaseTool::getCharNameFromID(playerID) + " dropped " + item);
		return ( "You look through your bag, but you can't find any " + keyword + "." );
	}
	return ( "You dropped " + keyword + "." );
}









