#include <char/Character.hpp>
#include "DatabaseTool.hpp"
#include <GameCode.hpp>
#include "Server.hpp"


// ------------------- PUBLIC -------------------

std::pair< std::string, std::string > Character::performCommand( int charId, Command command ) {
	const std::string CMD_LOOK		= "look";
	const std::string CMD_LOOK_AT	= "look at";
	
	if ( command.type == CMD_LOOK ) {
		return std::make_pair( GameCode::STATUS, Character::look( charId, command.data ) );
	}
}


std::string Character::getStats( int charId ) {
	Attributes attributes = DatabaseTool::getAttributes( charId, Target:character );
	
	std::string formattedStats = "";
	
	std::cout << formattedStats << std::endl;
	return formattedStats;
}


void Character::updateStats( int charId ) {
	std::string formattedStats;
	
	Server::sendMessageToCharacter( recipientId, GameCode::CHAT_PRIVATE, formattedMessage );
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