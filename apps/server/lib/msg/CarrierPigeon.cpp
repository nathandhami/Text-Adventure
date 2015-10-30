#include "CarrierPigeon.hpp"
#include "Server.hpp"

#include <iostream>


// ------------------- PUBLIC -------------------

int CarrierPigeon::deliverPackage( int senderId/*, Command messageCommand*/ ) {
	//TO-DO: check for command type, and act accordingly
	
	
	
	CarrierPigeon::deliverToCharacter( senderId, "@ testChar1 wolololo." );
	
	return 0;
}



// ------------------- PRIVATE ------------------

int CarrierPigeon::deliverToZone( int senderId, std::string message ) {
	std::string messagePrefix = "[zone] scrub: ";
	
	//TO-DO: get all char ids in the sender's zone
	
	return 0;
}


int CarrierPigeon::deliverToCharacter( int senderId, std::string message ) {
	const int ONE_CHARACTER = 1;
	
	//TO-DO: retrieve char name from the message
	
	int recipientId = 1;
	
	
	std::string messagePrefix = "[private] scrub: ";
	std::string formattedMessage = messagePrefix + message;
	
	bool delivered = Server::sendMessageToCharacter( recipientId, GameCode::CHAT_PRIVATE, formattedMessage );
	
	if ( delivered ) {
		std::cout << "Pigeon flew!" << std::endl;
		return ONE_CHARACTER;
	}
	
	std::cout << "Pigeon crashed!" << std::endl;
	return 0;
}