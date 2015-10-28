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


	static std::tuple< std::string, Command >  getHeaderAndCommand(std::string command);
    static std::tuple< std::string, Command > getHeaderAndCommandFromString(std::string commandString);


private:

};
#endif