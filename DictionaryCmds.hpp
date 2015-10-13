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
    DictionaryCmds();
    
    ~DictionaryCmds(); // destructor
    
    int dictionarySize() const; // returns number of commands in dictionary
    
    bool checkCommandValid(Command commandType) const; // checks if a command
                                                       // exists in the dictionary
    //getter function for commandsInDictionary map
    std::map<std::string, std::string> getDictionary() const;


private:
    std::map <std::string, std::string> commandsInDictionary;
};



//#endif

