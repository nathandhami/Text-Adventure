#include <mod/WorldEditor.hpp>
#include <DatabaseTool.hpp>

#include <iostream>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/regex.hpp>
#include <boost/regex.hpp>
#include <vector>

// Useful Macros
#define LOG( msg ) std::cout << "[WorldEditor] " << msg << std::endl



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


void WorldEditor::createZone( std::string zoneData ) {
	const int STAGE_NAME = 	0;
	const int STAGE_DESC = 	1;
	
	const std::string STAGE_SEP = ">>";
	
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
	
	int zoneid = DatabaseTool::createNewZone( zoneName, zoneDesc );
	LOG( "Insert zone with ID: " << zoneid );
	LOG( " == Done parsing." );
	
}


void WorldEditor::describeZone( int creatorId, std::string zoneData ) {
	const int EXPECTED_TOKEN_NUM = 3;
	boost::regex pattern( "(~as)|(~:)" );
	
	std::vector< std::string > tokens;
	parseToTokens( tokens, pattern, zoneData );
	
	LOG( "Zone ID: " << tokens[0] );
	LOG( "Desc: " << tokens[1] );
	LOG( "Keywords: " << tokens[2] );
	
//	std::vector< std::string > commandTokens;
//	boost::algorithm::split_regex( commandTokens, zoneData, boost::regex( "~as" ) );
//
//	std::vector< std::string > descTokens;
//	boost::algorithm::split_regex( descTokens, commandTokens[ 1 ], boost::regex( "~:" ) );
//	
//	int zoneId = atoi( commandTokens[ 0 ].c_str() );
//	
//	LOG( "Zone ID: " << commandTokens[ 0 ] );
//	LOG( "Zone ID: " << zoneId );
//	
	DatabaseTool::addExtendedDescriptionToZone( atoi( tokens[ 0 ].c_str() ), tokens[ 1 ], tokens[ 2 ] );
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












