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

DictionaryCmds::DictionaryCmds() {
    
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



