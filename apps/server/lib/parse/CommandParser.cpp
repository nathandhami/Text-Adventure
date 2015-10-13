/*
 * CommandParser.cpp
 *
 *  This class gets the player ID and unparsed user command from the controller
 *  and parses it into the type of command it is, and the value of the data for that command
 */
#include <iostream>
#include "CommandParser.hpp"
#include <string>
#include "NetConfig.hpp"
#include "DictionaryCmds.hpp"

std::string CommandParser::handleIDandCommand(int playerID, std::string command){

	Command parsedCommand = getCommandFromString( command );//Parse user command
	if ( parsedCommand.type == HEADER_ERROR ) return HEADER_ERROR;
	std::string stringToReturn;
	//if (DictionaryCmds::checkCommandValid(parsedCommand)){//check command is valid
	stringToReturn = World::executeCommand( playerID, parsedCommand );//gets response from world class to send to controller
	std::cout << parsedCommand.type << std::endl;
	//returns the reply from the world to the controller
	return stringToReturn;
}


//takes user command in string form and parses it into a Command struct
Command CommandParser::getCommandFromString( std::string commandString ) {
	Command parsedCommand;
	std::string parsableString = DictionaryCmds::getParsableFromInput( commandString );
	if ( parsableString == DictionaryCmds::INVALID_COMMAND ) {
		parsedCommand.type = HEADER_ERROR;
		return parsedCommand;
	}
	
	std::vector< std::string > tokens;
	boost::split( tokens, parsableString, boost::is_any_of( ";" ) );
	
	parsedCommand.type = tokens[ 0 ];
	parsedCommand.data = tokens[ 1 ];
	
	return parsedCommand;
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
