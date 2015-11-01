//
//  DictionaryCmds.cpp
//  textAdventure
//
//  Created by kenan abdulkarim on 2015-10-12.
//  Copyright © 2015 kenan abdulkarim. All rights reserved.
//

#include "DictionaryCmds.hpp"
#include <map>
#include <utility>
#include <iostream>
#include <string>

using namespace std;

const enum CommandHeader DictionaryCmds::INVALID_COMMAND = INVALID;

std::map< std::string, enum CommandHeader > DictionaryCmds::COMMAND_DICTIONARY = {
	//World header 
	{ "move"			 , WORLD },
	{ "north"			 , WORLD },
	{ "south"			 , WORLD },
	{ "east"			 , WORLD },
	{ "west"		   	 , WORLD },
	{ "go"	 			 , WORLD },
	{ "northeast"	 	 , WORLD },
	{ "northwest"		 , WORLD },
	{ "southeast"		 , WORLD },
	{ "southwest"   	 , WORLD },
	{ "look"		     , WORLD },
	{ "look at"			 , WORLD },
	{ "pick up" 		 , WORLD },
	//Combat Header 
	{ "fight"			 , COMBAT },
	{ "attack"			 , COMBAT },
	{ "retreat"			 , COMBAT },
	{ "accept challenge" , COMBAT },
	// EDITOR header
	{ "create zone" 	 , CHARACTER },
	{ "describe zone" 	 , CHARACTER },	
	{ "create door in" 	 , CHARACTER },
	{ "create item" 	 , CHARACTER },
	{ "create doodad" 	 , CHARACTER },
	// Character header
	{ "equip" 		     , CHARACTER },
	{ "pick up" 		 , CHARACTER },
	// Incomplete header
	{ "accept" 			 , INCOMPLETE },
	{ "pick" 			 , INCOMPLETE },
	{ "describe" 		 , INCOMPLETE },
	{ "create door" 	 , INCOMPLETE },
	{ "create" 			 , INCOMPLETE },

	// Messenger header
	{ "@"				 , MESSENGER },
	{ "#"  				 , MESSENGER },
};



/*DictionaryCmds::DictionaryCmds() {
    
    //commandsInDictionary[0] = "move";
    commandsInDictionary["move"] = "move";
    commandsInDictionary["north"] = "move north";
    commandsInDictionary["south"] = "move south";
    commandsInDictionary["east"] = "move east";
    commandsInDictionary["west"] = "move west";
    commandsInDictionary["south"] = "move south";
   // commandsInDictionary["login"] = "user login";
}

DictionaryCmds::~DictionaryCmds() {


}

int DictionaryCmds::dictionarySize() const {
    return commandsInDictionary.size();
}

bool DictionaryCmds::checkCommandValid(Command commandType) const {

	if (commandsInDictionary.count(commandType.type)) {
		return true;
	}
	else {
		return false;
	}
}

std::map<std::string, std::string> DictionaryCmds::getDictionary() const {
    return commandsInDictionary;
}*/

enum CommandHeader DictionaryCmds::getParsableFromInput( std::string inputString ) {
	if ( !DictionaryCmds::COMMAND_DICTIONARY.count( inputString ) ) {
		return DictionaryCmds::INVALID_COMMAND;
	}
	std::cout << "Parsable: " << DictionaryCmds::COMMAND_DICTIONARY[ inputString ] << std::endl;
	return DictionaryCmds::COMMAND_DICTIONARY[ inputString ]; // must return enum here
}










//void DictionaryCmds::processCommand(const std::string command) const {
//	if (command.compare("move north") == 1) {
//		std::cout << "player moves to the north and sees something!!" <<endl;
//	}
//	else if (command.compare("north") == 1) {
//		std::cout << "player moves to the north and sees something!!" <<endl;
//	}
//	else if (command.compare("login") == 1) {
//		std::cout << "user (put the user name here) logs in" <<endl;
//	}
//	else if (command.compare("logout") == 1) {
//		std::cout << "user (put the user name here) logs out" <<endl;
//	}
//}



