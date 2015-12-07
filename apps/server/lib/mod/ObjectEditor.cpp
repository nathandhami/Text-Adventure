#include <mod/ObjectEditor.hpp>
#include "DatabaseTool.hpp"

#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/regex.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/regex.hpp>
#include <vector>
#include <iostream>


#define LOG( msg ) std::cout << "[ObjectEditor] " << msg << std::endl


static void parseToTokens( std::vector< std::string >& tokens, boost::regex pattern, std::string parseable ) {
	boost::algorithm::split_regex( tokens, parseable, pattern );
	for ( std::string& token: tokens ) {
		boost::trim( token );
	}
}


// create item [item name] seen as [what you see] upclose [the long description] recognized as [keywords]
std::string ObjectEditor::createItem( int creatorId, std::string itemData ) {
	const std::string MSG_INVALID_CMD = "Incorrect creation command.";
	const int NUM_EXP_ARGS = 4;
	boost::regex PATTERN( "(\\]((?!\\]).)*?seen as.*?\\[)|(\\]((?!\\]).)*?upclose.*?\\[)|(\\]((?!\\]).)*?recognized as.*?\\[)" );
	
	boost::trim_if( itemData, boost::is_any_of( "[] " ) );
	
	std::vector< std::string > tokens;
	boost::algorithm::split_regex( tokens, itemData, PATTERN );
	
	std::string responseMessage = MSG_INVALID_CMD;
	if ( tokens.size() == NUM_EXP_ARGS ) {
		std::string name = tokens[ 0 ];
		std::string description = tokens[ 1 ];
		std::string longDescription = tokens[ 2 ];
		std::string keywords = tokens[ 3 ];
		
		//string shrtDesc, string desc, string lngDesc, string keywords 
		int itemId = DatabaseTool::createNewItem( name, description, longDescription, keywords );
		responseMessage = "Created item \"" + name + "\" [" + std::to_string( itemId ) + "]";
	}
	
	return responseMessage;
}


std::string ObjectEditor::deleteObject( std::string objectData ) {
	const std::string MSG_INVALID_CMD = "This is not a valid ID.";
	
	std::string responseMessage = MSG_INVALID_CMD;
	try {
		int objectId = boost::lexical_cast< int >( objectData );
		if ( objectId ) {
			DatabaseTool::deleteObject( objectId );
			responseMessage = "Deleted object " + std::to_string( objectId );
		}
	} catch( boost::bad_lexical_cast const& ) {
		std::cout << "Object ID can only be a number." << std::endl;
	}
	
	return responseMessage;
}


std::string ObjectEditor::placeObject( int creatorId, std::string objectData ) {
	const std::string MSG_INVALID_CMD = "This is not a valid ID.";
	
	try {
		int objectId = boost::lexical_cast< int >( objectData );
		int zoneId = DatabaseTool::getCharsLocation( creatorId );
		
		if ( DatabaseTool::spawnItemInZone( objectId, zoneId ) ) {
			return ( "Spawned item [" + objectData + "] at your position." );
		} else {
			return MSG_INVALID_CMD;
		}
	} catch( boost::bad_lexical_cast const& ) {
		return MSG_INVALID_CMD;
	}
}