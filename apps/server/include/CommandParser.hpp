#ifndef COMMAND_PARSER_H
#define COMMAND_PARSER_H

#include <iostream>
#include "World.hpp"
#include "Command.hpp"
#include <tuple>
#include <string>
#include "DictionaryCmds.hpp"



class CommandParser {
	
public:
	CommandParser();
	~CommandParser();


	static std::tuple< enum CommandHeader, Command >  getHeaderAndCommand( std::string command );
    static std::tuple< enum CommandHeader, Command > getHeaderAndCommandFromString( std::string commandString );

private:
	static bool checkIfCardinal( std::string token );
};
#endif