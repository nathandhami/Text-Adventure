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

//struct Command{

// std::string type;
// std::string data;
//};



const enum CommandHeader DictionaryCmds::INVALID_COMMAND = INVALID;

std::map< std::string, enum CommandHeader > DictionaryCmds::COMMAND_DICTIONARY = {
	//Movement commands
	{ "move"			, WORLD },
	{ "north"			, WORLD },
	{ "south"			, WORLD },
	{ "east"			, WORLD },
	{ "west"			, WORLD },
	{ "northeast"		, WORLD },
	{ "northwest"		, WORLD },
	{ "southeast"		, WORLD },
	{ "southwest"		, WORLD },
	{ "look"			, WORLD },
	{ "look at"			, WORLD },

	{ "create zone"		, EDITOR },
	{ "describe zone"	, EDITOR },
	{ "create door in"	, EDITOR },
	{ "create item"		, EDITOR },
	{ "create doodad"	, EDITOR },

	{ "fight"			, COMBAT },
	{ "attack"			, COMBAT },
	{ "retreat"			, COMBAT },
	{ "accept challenge", COMBAT },

	{ "pick"			, INCOMPLETE },
	{ "create"			, INCOMPLETE },
	{ "describe"		, INCOMPLETE },
	{ "create door"		, INCOMPLETE },
	{ "accept"			, INCOMPLETE },

	{ "equip"			, CHARACTER },
	{ "pick up"			, CHARACTER },

	{ "@"				, MESSENGER },
	{ "#"				, MESSENGER }
};



enum CommandHeader DictionaryCmds::getParsableFromInput( std::string inputString ) {
	if ( !DictionaryCmds::COMMAND_DICTIONARY.count( inputString ) ) {
		return DictionaryCmds::INVALID_COMMAND;
	}
	std::cout << "Parsable: " << DictionaryCmds::COMMAND_DICTIONARY[ inputString ] << std::endl;
	return DictionaryCmds::COMMAND_DICTIONARY[ inputString ];
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



