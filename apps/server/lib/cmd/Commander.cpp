#include <cmd/Commander.hpp>
#include "CommandParser.hpp"
#include "GameCode.hpp"
#include "CarrierPigeon.hpp"
#include <mod/Editor.hpp>


// ------------------- PRIVATE ------------------




// ------------------- PUBLIC -------------------

std::pair< std::string, std::string > Commander::handleCommand( User user, std::string commandText ) {
	const std::string MESSAGE_INVALID = "Invalid command: " + commandText;
	
	std::string responseHeader = GameCode::INVALID;
	std::string responseBody = MESSAGE_INVALID;
	
	std::tuple< int, Command > parserResponse = ( CommandParser::getHeaderAndCommand( commandText ) );
	int commandHeader = std::get< 0 >( parserResponse );
	Command command = std::get< 1 >( parserResponse );
	
	if ( commandHeader == CommandHeader::WORLD ) {
		std::string worldResponse =  World::executeCommand( user.getUserId(), command );
		responseHeader = GameCode::DESCRIPTION;
		responseBody = worldResponse;
	} else if ( commandHeader == CommandHeader::MESSENGER ) {
		int numSentTo = CarrierPigeon::deliverPackage( user.getUserId(), command );
		
		if ( !numSentTo ) {
			responseHeader = GameCode::STATUS;
			responseBody = "There is nobody to send this to.";
		} else {
			responseHeader = GameCode::OK;
			responseBody = "Message delivered to " + std::to_string( numSentTo ) + " players.";
		}
	} else if ( commandHeader == CommandHeader::EDITOR ) {
		std::string worthyMessage = Editor::judgeAndPerform( user.getUserId(), user.getUserId(), command );
		
		responseHeader = GameCode::STATUS;
		responseBody = worthyMessage;
	}
	
	return std::make_pair( responseHeader, responseBody );
}