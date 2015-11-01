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
//#include "CommandParser.hpp"




class DictionaryCmds {
    
public:
	
	static const std::string INVALID_COMMAND;
	
	// Interpret user's input and return parsable string
	static std::string getParsableFromInput( std::string inputString );
    
//    static int dictionarySize() const; // returns number of commands in dictionary
    
//    static bool checkCommandValid(Command commandType) const; // checks if a command
                                                       // exists in the dictionary
    //getter function for commandsInDictionary map
//    static std::map<std::string, std::string> getDictionary() const;
  enum CommandHeader {WORLD, COMBAT, MESSENGER, INCOMPLETE, INVALID, CHARACTER, COMBAT};

private:
    static std::map <std::string, std::string> COMMAND_DICTIONARY;
};



#endif