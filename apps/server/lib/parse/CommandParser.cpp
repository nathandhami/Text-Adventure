#include <iostream>
#include "CommandParser.hpp"
#include <string>

std::string CommandParser::handleIDandCommand(int playerID, std::string command){

	Command parsedCommand = getCommandFromString(command);
	if (bool checkCommandValid(parsedCommand)){

	}
	std::string stringToReturn;
	return stringToReturn;
}



Command CommandParser::getCommandFromString(std::string commandString){//takes user command in string form and parses it into a Command struct

	std::string splitString[2];
	std::string tempStr;
	char delim = '0';
	int stringIndex = 0;
	int arrayIndex = 0;

	while (stringIndex < commandString.size()){ //splits the command string into the type and data
		delim = commandString.at(stringIndex);
		if (delim != ';'){
			tempStr.at(stringIndex) = delim;
		}else if (delim == ';' && arrayIndex < 2){
			splitString[arrayIndex] = tempStr;
			arrayIndex++;
			tempStr = "";
		}
		stringIndex++;
	}

	Command parsedCommand;
	stringIndex = 0;
	tempStr = "";
	bool delimFound = false;
	std::string commandType = splitString[0];
	std::string commandData = splitString[1];

	while (stringIndex < commandType.size()){//take command type string and 
		delim = commandType.at(stringIndex);
		if(delim == ':'){
			delimFound = true;
		}
		if(delim != ':' && delimFound == true){
			tempStr.at(stringIndex) = delim;
		}
		stringIndex++;
	}
	parsedCommand.type = tempStr;

	stringIndex = 0;
	tempStr = "";
	bool delimFound = false;

	while (stringIndex < commandType.size()){//take command type string and 
		delim = commandType.at(stringIndex);
		if(delim == ':'){
			delimFound = true;
		}
		if(delim != ':' && delimFound == true){
			tempStr.at(stringIndex) = delim;
		}
		stringIndex++;
	}
	parsedCommand.type = tempStr;

	return parsedCommand;
	
}
