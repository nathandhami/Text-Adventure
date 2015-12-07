#include <cmd/Commander.hpp>
#include "CommandParser.hpp"
#include "GameCode.hpp"
#include "CarrierPigeon.hpp"
#include <mod/Editor.hpp>
#include "Combat.hpp"
#include "Spellcasting.hpp"
#include <char/Character.hpp>


// ------------------- PRIVATE ------------------




// ------------------- PUBLIC -------------------

std::pair< std::string, std::string > Commander::handleCommand( User user, std::string commandText ) {
	const std::string MESSAGE_INVALID = "Invalid action: " + commandText;
	
	std::string responseHeader = GameCode::INVALID;
	std::string responseBody = MESSAGE_INVALID;
	
	std::tuple< int, Command > parserResponse = ( CommandParser::getHeaderAndCommand( commandText ) );
	int commandHeader = std::get< 0 >( parserResponse );
	Command command = std::get< 1 >( parserResponse );
	
	std::cout << "Got command." << std::endl;
	
	if ( commandHeader == CommandHeader::CHARACTER ) {
		return Character::performCommand( user.getSelectedCharacterId(), command );
	} else if ( commandHeader == CommandHeader::MESSENGER ) {
		int numSentTo = CarrierPigeon::deliverPackage( user.getSelectedCharacterId(), command );
		if ( !numSentTo ) {
			responseHeader = GameCode::STATUS;
			responseBody = "There is nobody to send this to.";
		} else {
			responseHeader = GameCode::OK;
			responseBody = "Message delivered to " + std::to_string( numSentTo ) + " players.";
		}
	} else if ( commandHeader == CommandHeader::COMBAT ) {
		std::string combatMessage = Combat::executeCommand( user.getUserId(), command );
		
		responseHeader = GameCode::COMBAT;
		responseBody = combatMessage;
	} else if ( commandHeader == CommandHeader::EDITOR ) {
		std::string worthyMessage = Editor::judgeAndPerform( user.getUserId(), user.getUserId(), command );
		
		responseHeader = GameCode::STATUS;
		responseBody = worthyMessage;
	} else if ( commandHeader == CommandHeader::CASTING ) {
		std::string castingMessage = Spellcasting::executeCommand( user.getUserId(), command );

		responseHeader = GameCode::COMBAT;
		responseBody = castingMessage;
	}
	
	return std::make_pair( responseHeader, responseBody );
	
//	if ( commandHeader == CommandHeader::WORLD ) {
//		std::string worldResponse =  World::executeCommand( user.getUserId(), command );
//		responseHeader = GameCode::DESCRIPTION;
//		responseBody = worldResponse;
//	} else if ( commandHeader == CommandHeader::MESSENGER ) {
//		int numSentTo = CarrierPigeon::deliverPackage( user.getUserId(), command );
//		
//		if ( !numSentTo ) {
//			responseHeader = GameCode::STATUS;
//			responseBody = "There is nobody to send this to.";
//		} else {
//			responseHeader = GameCode::OK;
//			responseBody = "Message delivered to " + std::to_string( numSentTo ) + " players.";
//		}
//	} else if ( commandHeader == CommandHeader::EDITOR ) {
//		std::string worthyMessage = Editor::judgeAndPerform( user.getUserId(), user.getUserId(), command );
//		
//		responseHeader = GameCode::STATUS;
//		responseBody = worthyMessage;
//	} else if ( commandHeader == CommandHeader::COMBAT ) {
//		std::string combatMessage = Combat::executeCommand( user.getUserId(), command );
//		
//		responseHeader = GameCode::COMBAT;
//		responseBody = combatMessage;
//	} else if ( commandHeader == CommandHeader::CHARACTER ) {
//		return Character::performCommand( user.getUserId(), command );
//	}
	
	return std::make_pair( responseHeader, responseBody );
}