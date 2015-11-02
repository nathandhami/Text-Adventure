#include "CarrierPigeon.hpp"
#include "Server.hpp"
#include "DatabaseTool.hpp"
#include "Command.hpp"

#include <iostream>
#include <boost/algorithm/string.hpp>


#define LOG( msg ) std::cout << "[Pigeon] " << msg << std::endl


// ------------------- PUBLIC -------------------

int CarrierPigeon::deliverPackage( int senderId, Command messageCommand ) {
	const std::string PRIV_MSG( "@" );
	const std::string ZONE_MSG( "#" );
	//TO-DO: check for command type, and act accordingly
	int numDeliveredTo = 0;
	if ( messageCommand.type == PRIV_MSG ) {
		CarrierPigeon::deliverToCharacter( senderId, messageCommand.data );
	} else if ( messageCommand.type == ZONE_MSG ) {
		CarrierPigeon::deliverToZone( senderId, messageCommand.data );
	} else {
		LOG( "The pigeon can't decide where to fly and crashes into a pole." );
	}
	
	
//	CarrierPigeon::deliverToCharacter( senderId, "leeeroooyjeeenkins wolololo." );
//	CarrierPigeon::deliverToZone( senderId, "wolololo." );
	
	return 0;
}



// ------------------- PRIVATE ------------------

int CarrierPigeon::deliverToZone( int senderId, std::string message ) {
	
	//TO-DO: get all char ids in the sender's zone
	
	int zoneId = DatabaseTool::getCharsLocation( senderId );
	
	std::cout << "PEGION; zoneid: " << zoneId << std::endl;
	
	std::vector< int > characterIds = DatabaseTool::getAllOnlineCharsInZone( zoneId );
	
	
	
	std::string senderName = DatabaseTool::getCharNameFromID( senderId );
	
	std::string messagePrefix = "[zone] " + senderName + ": ";
	std::string formattedMessage = messagePrefix + message;
	
	int numDelivered = 0;
	for ( int& recipientId: characterIds ) {
		std::cout << "PEGION; id: " << recipientId << std::endl;
		bool delivered = Server::sendMessageToCharacter( recipientId, GameCode::CHAT_ZONE, formattedMessage );
		if ( delivered ) {
			std::cout << "Pigeon flew!" << std::endl;
			numDelivered ++;
		}
	}
	
	return numDelivered;
}


int CarrierPigeon::deliverToCharacter( int senderId, std::string message ) {
	const int ONE_CHARACTER = 1;
	
	std::vector< std::string > tokens;
	boost::split( tokens, message, boost::is_any_of( " " ) );
	
	std::string recipientName = tokens[ 0 ];
	std::string messageBody = message.substr( message.find_first_of( " " ) + 1 );
	
	int recipientId = DatabaseTool::getCharIDFromName( recipientName );
	
	std::string senderName = DatabaseTool::getCharNameFromID( senderId );
	
	std::string messagePrefix = "[private] " + senderName + ": ";
	std::string formattedMessage = messagePrefix + messageBody;
	
	bool delivered = Server::sendMessageToCharacter( recipientId, GameCode::CHAT_PRIVATE, formattedMessage );
	
	if ( delivered ) {
		std::cout << "Pigeon flew!" << std::endl;
		return ONE_CHARACTER;
	}
	
	std::cout << "Pigeon crashed!" << std::endl;
	return 0;
}