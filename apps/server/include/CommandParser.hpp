#ifndef COMMAND_PARSER_H
#define COMMAND_PARSER_H

#include <iostream>
#include "World.hpp"
#include "Command.hpp"
#include <tuple>
#include <string>




class CommandParser {
	
public:
	CommandParser();
	~CommandParser();


//	static std::tuple<  int, Command >  getHeaderAndCommand( std::string command );
//    static std::tuple<  int, Command > getHeaderAndCommandFromString( std::string commandString );
	
	static std::tuple< int, Command >  getHeaderAndCommand( std::string command );
//    static st getHeaderAndCommandFromString( std::string commandString );

private:
	static bool checkIfCardinal( std::string token );
};
#endif