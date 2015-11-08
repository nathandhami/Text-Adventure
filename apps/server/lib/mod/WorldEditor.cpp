#include <mod/WorldEditor.hpp>
#include <DatabaseTool.hpp>
#include "Zone.hpp"

#include <iostream>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/regex.hpp>
#include <boost/regex.hpp>
#include <boost/lexical_cast.hpp>
#include <vector>

// Useful Macros
#define LOG( msg ) std::cout << "[WorldEditor] " << msg << std::endl


// ------------------- PRIVATE ------------------

static std::string addTokenToString( std::string originalString, std::string token ) {
	if ( originalString.empty() ) {
		return ( token );
	} else {
		return ( originalString + " " + token );
	}
}


static void parseToTokens( std::vector< std::string >& tokens, boost::regex pattern, std::string parseable ) {
	boost::algorithm::split_regex( tokens, parseable, pattern );
	for ( std::string& token: tokens ) {
		boost::trim( token );
	}
}


static int getIntFromString( std::string text ) {
	try {
		int number = boost::lexical_cast<int>( text );
		return number;
	} catch( boost::bad_lexical_cast const& ) {
		return 0;
	}
}


// ------------------- PUBLIC -------------------

std::string WorldEditor::createZone( std::string zoneData ) {
	const std::string MSG_INVALID_CMD = "Incorrect creation command. Usage:\n\tcreate zone [name] seen as [description]";
	const int NUM_EXP_ARGS = 2;
	const boost::regex PATTERN( "(\\].*seen as.*\\[)" );
	
	boost::trim_if( zoneData, boost::is_any_of( "[] " ) );
	
	std::vector< std::string > tokens;
	boost::algorithm::split_regex( tokens, zoneData, PATTERN );
	
	std::string responseMessage = MSG_INVALID_CMD;
	if ( tokens.size() == NUM_EXP_ARGS ) {
		int zoneId = DatabaseTool::createNewZone( tokens[ 0 ], tokens[ 1 ] );
		responseMessage = "You created zone \"" + tokens[ 0 ] + "\" with ID: " + std::to_string( zoneId );
	}
	
	return responseMessage;
}


std::string WorldEditor::deleteZone( std::string zoneData ) {
	const std::string MSG_INVALID_CMD = "This is not a valid ID.";
	const int NUM_EXP_ARGS = 1;
	
	std::string responseMessage = MSG_INVALID_CMD;
	try {
		int zoneId = boost::lexical_cast<int>( zoneData );
		if ( zoneId ) {
			DatabaseTool::deleteZone( zoneId );
			responseMessage = "Deleted zone " + std::to_string( zoneId );
		}
	} catch( boost::bad_lexical_cast const& ) {
		std::cout << "Zone ID can only be a number." << std::endl;
	}
	
	return responseMessage;
}


std::string WorldEditor::describeZone( int creatorId, std::string zoneData ) {
	const std::string MSG_INVALID_CMD = "Incorrect creation command.";
	const int NUM_EXP_ARGS = 3;
	const boost::regex PATTERN( "(\\]((?!\\]).)*?as.*?\\[)|(\\]((?!\\]).)*?telling.*?\\[)" );
	
	boost::trim_if( zoneData, boost::is_any_of( "[] " ) );
	
	std::vector< std::string > tokens;
	boost::algorithm::split_regex( tokens, zoneData, PATTERN );
	
	std::string responseMessage = MSG_INVALID_CMD;
	if ( tokens.size() == NUM_EXP_ARGS ) {
		int zoneId = atoi( tokens[ 0 ].c_str() );
		std::string keyword = tokens[ 1 ];
		std::string description = tokens[ 2 ];
		
		DatabaseTool::addExtendedDescriptionToZone( zoneId, description, keyword );
		responseMessage = "You have bent the fabric of time itself.";
		Zone::broadcastMessage( zoneId, "You feel something changed about this place, but you can't quite put your finger on it." );
	}
	
	return responseMessage;
}


std::string WorldEditor::addDoorToZone( int creatorId, std::string doorData ) {
	const std::string MSG_INVALID_CMD = "Incorrect creation command.";
	const int NUM_EXP_ARGS = 5;
	const boost::regex PATTERN( "(\\]((?!\\]).)*?in.*?\\[)|(\\]((?!\\]).)*?at.*?\\[)|(\\]((?!\\]).)*?to.*?\\[)|(\\]((?!\\]).)*?seen as.*?\\[)" );
	
	boost::trim_if( doorData, boost::is_any_of( "[] " ) );
	
	std::vector< std::string > tokens;
	boost::algorithm::split_regex( tokens, doorData, PATTERN );
	
	std::string responseMessage = MSG_INVALID_CMD;
	if ( tokens.size() == NUM_EXP_ARGS ) {
		std::string doorName 		= tokens[ 0 ];
		int zoneId 					= atoi( tokens[ 1 ].c_str() );
		std::string direction 		= tokens[ 2 ];
		int destinationId 			= atoi( tokens[ 3 ].c_str() );
		std::string description 	= tokens[ 4 ];
		
		LOG( "ZONEID1: " << zoneId );
		LOG( "ZONEID2: " << destinationId );
		
		//int zoneID, string description, string direction, int pointer, string keywords
		int newDoorId = DatabaseTool::addDoorToZone( zoneId, description, direction, destinationId, doorName );
		responseMessage = "You have collided two zones! [" + std::to_string( newDoorId ) + "]";
		Zone::broadcastMessage( zoneId, "You feel like the moment you turned your head the world has grown larger!" );
	}
	
	return responseMessage;
}


std::string WorldEditor::deleteDoor( int creatorId, std::string doorData ) {
	const std::string MSG_INVALID_CMD = "This is not a valid ID.";
	
	std::string responseMessage = MSG_INVALID_CMD;
	int doorId = getIntFromString( doorData );
	if ( doorId ) {
		DatabaseTool::deleteDoor( doorId );
		responseMessage = "Deleted door " + std::to_string( doorId );
		int zoneId = DatabaseTool::getCharsLocation( creatorId );
		Zone::broadcastMessage( zoneId, "Watch your step, the world is shrinking!" );
	}
	
	return responseMessage;
}











