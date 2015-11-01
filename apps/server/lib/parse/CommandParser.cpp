/*
 * CommandParser.cpp
 *
 *  This class gets the raw command from the user
 *  and parses it into the type of command it is, 
 *	and the value of the data for that command
 *  
 *  It returns a header to the controller so it knows where to send the command to,
 *  along with the type of command, and the data for the command.
 */
#include <iostream>
#include "CommandParser.hpp"
#include "NetConfig.hpp"
#include "DictionaryCmds.hpp"

std::tuple< enum CommandHeader, Command > CommandParser::getHeaderAndCommand( std::string command ){

	std::tuple< enum CommandHeader, Command > headerAndParsedCommand;
	headerAndParsedCommand = getHeaderAndCommandFromString( command );//Parse user command and get a header associated with the command type

	//returns the header type and the command 
	return headerAndParsedCommand;
}
bool CommandParser::checkIfCardinal( std::string token ){ //checks if the command was entered as a (inter)cardinal direction only (no "move" in front)
	return( (      token == "north" ) 
			  || ( token == "south" )
			  || ( token == "east" )
			  || ( token == "west" )
			  || ( token == "northeast" )
			  || ( token == "northwest" )
			  || ( token == "southeast" )
			  || ( token == "southwest" ) );
}

//takes user command in string form and parses it into a Command struct
std::tuple< enum CommandHeader, Command > CommandParser::getHeaderAndCommandFromString( std::string commandString ) {
	
	Command parsedCommand;
	std::tuple< enum CommandHeader, Command > headerAndParsedCommand;
	enum CommandHeader commandHeader;
	std::vector< std::string > tokens;
	boost::split( tokens, commandString, boost::is_any_of( " " ),boost::token_compress_on );
	for( int i =0; i<tokens.size();i++ ){//testing
		cout << tokens[i] << endl;
	}

	enum CommandHeader header;
	int startOfCommandData;
	int i = 0;
	bool isACardinal;
	bool incompleteCommand;
	std::string lowerCaseToken;

	while ( header != INVALID ){
		lowerCaseToken = tokens[0];
		boost::to_lower( lowerCaseToken );
		isACardinal = checkIfCardinal( lowerCaseToken );
		header = DictionaryCmds::getParsableFromInput( lowerCaseToken );
		if(header == INCOMPLETE){ //check if it is an incomplete command
			incompleteCommand = true;
		}
		if( header != INVALID ){ //valid (possibly incomplete) command
			if( isACardinal ){ 
				parsedCommand.type = "move"; //set the command type to move if only (inter)cardinal entered
			}else{
				parsedCommand.type = tokens[0];
				if( i+1 < tokens.size() ){//checks to make sure it is not out of bounds
					tokens[0]= tokens[0] + " " + tokens[i+1]; //concatinate the first token with the next to retest if longer command name (eg "look at")
				}else header == INVALID;
			}
			commandHeader = header;
			
		}else if( ( header == INVALID ) && ( i > 0 ) ){ //invalid header found after finding an initially valid command
			if( isACardinal ){
				startOfCommandData = 0;//data starts at beginning instead of after command since it is a cardinal direction
			}else{
				startOfCommandData = i;// data starts after the command type is found
			}
		}else if( ( header == INVALID ) && ( ( i = 0 ) || ( incompleteCommand ) ) ){
			commandHeader = header;
			startOfCommandData = tokens.size();//invalid command so data becomes as small as possible since it wont be used
		}
		i++;
	}
	for ( int i = startOfCommandData; i<tokens.size();i++ ){ // concatinates data fields into one space seperated string
		if( parsedCommand.data.empty() ){
			parsedCommand.data = tokens[i];
		}else parsedCommand.data += " " + tokens[i];
	}

	headerAndParsedCommand = std::make_tuple ( commandHeader, parsedCommand );
	
	return headerAndParsedCommand;
	
}
