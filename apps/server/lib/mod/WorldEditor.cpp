#include <mod/WorldEditor.hpp>
#include <DatabaseTool.hpp>

#include <iostream>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/regex.hpp>
#include <boost/regex.hpp>
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
	const int STAGE_NAME = 	0;
	const int STAGE_DESC = 	1;
	
	int parsingStage = STAGE_NAME;
	
	std::string zoneName = "";
	std::string zoneDesc = "";
	
	LOG( " == Creating Zone..." );
	
	
	std::vector< std::string > tokens;
	boost::split( tokens, zoneData, boost::is_any_of( "|" ) );
	
	zoneName = tokens[ STAGE_NAME ];
	zoneDesc = tokens[ STAGE_DESC ];
	
	boost::trim( zoneName );
	boost::trim( zoneDesc );
	
	LOG( "Name: " << zoneName );
	LOG( "Description: " << zoneDesc );
	
	std::vector< Door > doors;
	
	int zoneId = DatabaseTool::createNewZone( zoneName, zoneDesc );
	LOG( "Insert zone with ID: " << zoneId );
	LOG( " == Done parsing." );
	
	return ( "Created \"" + zoneName + "\" with ID: " + std::to_string( zoneId ) );
	
}


std::string WorldEditor::describeZone( int creatorId, std::string zoneData ) {
	const int EXPECTED_TOKEN_NUM = 3;
	boost::regex pattern( "(~as)|(~:)" );
	
	std::vector< std::string > tokens;
	parseToTokens( tokens, pattern, zoneData );
	
	LOG( "Zone ID: " << tokens[ 0 ] );
	LOG( "Desc: " << tokens[ 1 ] );
	LOG( "Keywords: " << tokens[ 2 ] );

	DatabaseTool::addExtendedDescriptionToZone( atoi( tokens[ 0 ].c_str() ), tokens[ 1 ], tokens[ 2 ] );
	
	return ( "Described zone " + tokens[ 0 ] + " as " + tokens[ 1 ] );
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












