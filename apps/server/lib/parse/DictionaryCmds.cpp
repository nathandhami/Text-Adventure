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

using namespace CommandHeader;

//const  CommandHeader DictionaryCmds::INVALID_COMMAND = INVALID;

std::map< std::string,  int > DictionaryCmds::COMMAND_DICTIONARY = {
	//Movement commands
	{ "move"			, CHARACTER },
	{ "go"				, CHARACTER },
	{ "north"			, CHARACTER },
	{ "south"			, CHARACTER },
	{ "east"			, CHARACTER },
	{ "west"			, CHARACTER },
	{ "northeast"		, CHARACTER },
	{ "northwest"		, CHARACTER },
	{ "southeast"		, CHARACTER },
	{ "southwest"		, CHARACTER },
	{ "look"			, CHARACTER },
	{ "look at"			, CHARACTER },

	// EDITOR commands
	{ "create"			, INCOMPLETE },
	{ "create zone"		, EDITOR },
	{ "create door"		, EDITOR },
	{ "create item"		, EDITOR },
	{ "create doodad"	, EDITOR },
	
	{ "delete"			, INCOMPLETE },
	{ "delete zone"		, EDITOR },
	{ "delete door"		, EDITOR },
	{ "delete object"	, EDITOR },

	{ "describe"		, INCOMPLETE },
	{ "describe zone"	, EDITOR },

	{ "fight"			, COMBAT },
	{ "attack"			, COMBAT },
	{ "retreat"			, COMBAT },
	{ "accept challenge", COMBAT },
	{ "reject challenge", COMBAT },
	{ "retract challenge", COMBAT },

	{ "cast"			, CASTING },

	{ "pick"			, INCOMPLETE },
	{ "accept"			, INCOMPLETE },
	{ "reject"			, INCOMPLETE },
	{ "retract"			, INCOMPLETE },

	{ "equip"			, CHARACTER },
	{ "pick up"			, CHARACTER },
		
	{ "drop"			, CHARACTER },

	{ "@"				, MESSENGER },
	{ "#"				, MESSENGER }
};




int DictionaryCmds::getParsableFromInput( std::string inputString ) {
	if ( !DictionaryCmds::COMMAND_DICTIONARY.count( inputString ) ) {
		return INVALID;
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



