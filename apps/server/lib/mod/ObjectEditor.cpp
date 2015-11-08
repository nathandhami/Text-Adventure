#include <mod/ObjectEditor.hpp>
#include "DatabaseTool.hpp"

#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/regex.hpp>
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


void ObjectEditor::createItem( int creatorId, std::string itemData ) {
	const int EXPECTED_TOKEN_NUM = 4;
	boost::regex pattern( "(~:)|(~;)" );
	
	std::vector< std::string > tokens;
	parseToTokens( tokens, pattern, itemData );
	
	LOG( "Item name: " << tokens[0] );
	LOG( "Long desc: " << tokens[1] );
	LOG( "Desc: " << tokens[2] );
	LOG( "Keywords: " << tokens[3] );
	
	int itemId = DatabaseTool::createNewItem( tokens[ 0 ], tokens[ 2 ], tokens[ 1 ], tokens[ 3 ] );
	LOG( "Created item with ID: " << itemId );
}