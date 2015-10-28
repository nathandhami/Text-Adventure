/*
 * CommandParser.cpp
 *
 *  This class gets the player ID and unparsed user command from the controller
 *  and parses it into the type of command it is, and the value of the data for that command
 */
#include <iostream>
#include "CommandParser.hpp"
#include "NetConfig.hpp"
#include "DictionaryCmds.hpp"

std::tuple< std::string, Command > CommandParser::getHeaderAndCommand( std::string command ){

	std::tuple< std::string, Command > headerAndParsedCommand;
	headerAndParsedCommand = getHeaderAndCommandFromString( command );//Parse user command and get a header associated with the command type

	//returns the header type and the command 
	return headerAndParsedCommand;
}


//takes user command in string form and parses it into a Command struct
std::tuple< std::string, Command > CommandParser::getHeaderAndCommandFromString( std::string commandString ) {
	
	Command parsedCommand;
	std::tuple< std::string, Command > headerAndParsedCommand;
	std::string commandHeader = "header";//TODO MAKE INT AND DEFINE IN DICTIONARY
	std::vector< std::string > tokens;
	boost::split( tokens, commandString, boost::is_any_of( " " ),boost::token_compress_on );
	for(int i =0; i<tokens.size();i++){//testing
		cout << tokens[i] << endl;
	}

	std::string header;
	int startOfCommandData;
	while (header != "invstr"){
		int i = 0;
		std::string lowerCaseToken = tokens[0];
		boost::to_lower( tokens[0] );
		header = DictionaryCmds::getParsableFromInput( lowerCaseToken );
		if(header != "invstr"){
			parsedCommand.type = tokens[0];
			tokens[0]= tokens[0] + " " + tokens[i+1]; //concatinate the first token with the next to retest if longer command name (eg "look at")
			commandHeader = header;
		}else if(header == "invstr"){
			startOfCommandData = i;
		}
	}
	for ( int i = startOfCommandData; i<tokens.size();i++ ){
		if( parsedCommand.data.empty() ){
			parsedCommand.data = tokens[i];
		}else parsedCommand.data += " " + tokens[i];
	}

	headerAndParsedCommand = std::make_tuple ( commandHeader, parsedCommand );
	
	return headerAndParsedCommand;
	//PSEUDO DICTIONARY 
	/*if ( commandString == "north" ) {
		parsedCommand.type = "move";
		parsedCommand.data = "north";
	} else if ( commandString == "up" ) { 
		parsedCommand.type = "look";
		parsedCommand.data = "north";
	} else {
		parsedCommand.type = HEADER_ERROR;
	}*/
	
	/*std::string splitString[2];//holds the command type in [0] and the data in [1]
	std::string tempStr;
	char delim = '0';
	int stringIndex = 0;
	int arrayIndex = 0;

	
	
	while (stringIndex < commandString.size()){ //splits the command string into the type and data
		delim = commandString.at(stringIndex);
		if (delim != ';'){
//			tempStr.at(stringIndex) = delim;//builds the parsed string
			tempStr.append(&delim);
		}else if (delim == ';' && arrayIndex < 2){
			splitString[arrayIndex] = tempStr;
			arrayIndex++;
			tempStr = "";
		}
		stringIndex++;
	}

	Command parsedCommand;
	//reset variables for loop
	stringIndex = 0;
	tempStr = "";
	bool delimFound = false;
	std::string commandType = splitString[0];
	std::string commandData = splitString[1];

	while (stringIndex < commandType.size()){//takes the part of the string left after the ":" for the command type
//		std::cout << commandString.at(stringIndex) << std::endl;
		delim = commandType.at(stringIndex);
		if(delim == ':'){
			delimFound = true;
		}
		if(delim != ':' && delimFound == true){
//			tempStr.at(stringIndex) = delim;
			tempStr.append(&delim);
		}
		stringIndex++;
	}
	parsedCommand.type = tempStr;
	//reset variables for loop
	stringIndex = 0;
	tempStr = "";
	delimFound = false;

	while (stringIndex < commandType.size()){//takes the part of the string left after the ":" for the command data
		delim = commandType.at(stringIndex);
		if(delim == ':'){
			delimFound = true;
		}
		if(delim != ':' && delimFound == true){
//			tempStr.at(stringIndex) = delim;
			tempStr.append(&delim);
		}
		stringIndex++;
	}
	parsedCommand.data = tempStr;
*/
	
	//returns the type and data fields 
	
}
