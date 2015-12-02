#ifndef COMMAND_PARSER_H
#define COMMAND_PARSER_H

#include <iostream>
#include "World.hpp"
#include "Command.hpp"
#include <tuple>
#include <string>

namespace CommandHeader {
	static const int WORLD = 0;
	static const int COMBAT = 1;
	static const int MESSENGER = 2;
	static const int INCOMPLETE = 3;
	static const int INVALID = 4;
	static const int CHARACTER = 5;
	static const int EDITOR = 6;
	static const int CASTING = 7;
}


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