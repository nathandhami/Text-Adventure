#ifndef DICTIONARYCMDS_HPP
#define DICTIONARYCMDS_HPP

//
//  DictionaryCmds.hpp
//  textAdventure
//
//  Created by kenan abdulkarim on 2015-10-12.
//  Copyright © 2015 kenan abdulkarim. All rights reserved.
//

//#ifndef Included_DictionaryCmds
//#define Included_DictionaryCmds

//#include <stdio.h>
#include <iostream>
#include <string>
#include <map>
#include "CommandParser.hpp"
//#include "CommandParser.hpp"

//enum CommandHeader {WORLD, COMBAT, MESSENGER, INCOMPLETE, INVALID, CHARACTER, EDITOR};

//namespace CommandHeader {
	static const int WORLD = 0;
	static const int COMBAT = 1;
	static const int MESSENGER = 2;
	static const int INCOMPLETE = 3;
	static const int INVALID = 4;
	static const int CHARACTER = 5;
	static const int EDITOR = 6;
//}

//using namespace CommandHeader;

class DictionaryCmds {
    
public:
	
//	static const  int INVALID_COMMAND;
	
	// Interpret user's input and return parsable string
	static  int getParsableFromInput( std::string inputString );
    
//    static int dictionarySize() const; // returns number of commands in dictionary
    
//    static bool checkCommandValid(Command commandType) const; // checks if a command
                                                       // exists in the dictionary
    //getter function for commandsInDictionary map
//    static std::map<std::string, std::string> getDictionary() const;
//  enum CommandHeader {WORLD, COMBAT, MESSENGER, INCOMPLETE, INVALID, CHARACTER, COMBAT};

private:
    static std::map <std::string,  int> COMMAND_DICTIONARY;
};



#endif