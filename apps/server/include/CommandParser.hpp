#ifndef COMMAND_PARSER_H
#define COMMAND_PARSER_H
#include <iostream>
#include "World.hpp"
#include "DictionaryCmds.hpp"

typedef struct {

	std::string type;
	std::string data;

}Command;

class CommandParser {
	
public:
	CommandParser();
	~CommandParser();


	static std::string  handleIDandCommand(int playerID, std::string command);
    static Command getCommandFromString(std::string commandString);


private:

};
#endif