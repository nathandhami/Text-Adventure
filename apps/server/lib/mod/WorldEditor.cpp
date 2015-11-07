#include <mod/WorldEditor.hpp>
#include <DatabaseTool.hpp>

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
		if ( zoneId) {
			DatabaseTool::deleteZone( zoneId );
			responseMessage = "Deleted zone";
		}
	} catch( boost::bad_lexical_cast const& ) {
		std::cout << "Error: input string was not valid" << std::endl;
	}
	
	return responseMessage;
}


std::string WorldEditor::describeZone( int creatorId, std::string zoneData ) {
	const std::string MSG_INVALID_CMD = "Incorrect creation command.";
	const int NUM_EXP_ARGS = 3;
	const boost::regex PATTERN( "(\\].*as.*\\[)|(\\].*telling.*\\[)" );
	
	boost::trim_if( zoneData, boost::is_any_of( "[] " ) );
	
	std::vector< std::string > tokens;
	boost::algorithm::split_regex( tokens, zoneData, PATTERN );
	
	//TO-DO: finish what was started
	
//	const int EXPECTED_TOKEN_NUM = 3;
//	boost::regex pattern( "(~as)|(~:)" );
//	
//	std::vector< std::string > tokens;
//	parseToTokens( tokens, pattern, zoneData );
//	
//	LOG( "Zone ID: " << tokens[ 0 ] );
//	LOG( "Desc: " << tokens[ 1 ] );
//	LOG( "Keywords: " << tokens[ 2 ] );
//
//	DatabaseTool::addExtendedDescriptionToZone( atoi( tokens[ 0 ].c_str() ), tokens[ 1 ], tokens[ 2 ] );
//	
//	return ( "Described zone " + tokens[ 0 ] + " as " + tokens[ 1 ] );
}


void WorldEditor::addDoorToZone( int creatorId, std::string doorData ) {
	const int EXPECTED_TOKEN_NUM = 5;
	boost::regex pattern( "(~at)|(~to)|(~:)" );
	
	std::vector< std::string > tokens;
	parseToTokens( tokens, pattern, doorData );
	
	//TO-DO: check size, inform user if invalid format
	
	LOG( "Zone: " << tokens[0] );
	LOG( "Location: " << tokens[1] );
	LOG( "Destination: " << tokens[2] );
	LOG( "Description: " << tokens[3] );
	LOG( "Keywords: " << tokens[4] );
	
	DatabaseTool::addDoorToZone( atoi( tokens[ 0 ].c_str() ), tokens[ 3 ], tokens[ 1 ], atoi( tokens[ 2 ].c_str() ), tokens[ 4 ] );
}












