#include <char/Character.hpp>
#include "DatabaseTool.hpp"
#include <GameCode.hpp>
#include "Server.hpp"

#include <boost/algorithm/string/join.hpp>
#include <boost/algorithm/string.hpp>


// ------------------- PUBLIC -------------------

std::pair< std::string, std::string > Character::performCommand( int charId, Command command ) {
	const std::string CMD_LOOK		= "look";
	const std::string CMD_LOOK_AT	= "look at";
	
	if ( command.type == CMD_LOOK ) {
		return std::make_pair( GameCode::STATUS, Character::look( charId, command.data ) );
	} else if ( command.type == CMD_LOOK_AT ) {
		return std::make_pair( GameCode::STATUS, Character::lookAt( charId, command.data ) );
	}
}


std::string Character::getStats( int charId ) {
	Attributes attributes = DatabaseTool::getAttributes( charId, Target::character );
	
	std::string formattedStats = "";
	
	std::cout << formattedStats << std::endl;
	return formattedStats;
}


void Character::updateStats( int charId ) {
	std::string formattedStats;
	
	Server::sendMessageToCharacter( charId, GameCode::CHAT_PRIVATE, formattedStats );
}


// ------------------- PRIVATE ------------------

std::string Character::look( int charId, std::string direction ) {
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
	std::string description = "";
	
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