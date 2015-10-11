#include <iostream>
// TODO #include "World.h"
#define COMMAND_SIZE 2

typedef struct {

	std::string type;
	std::string data;

}Command;

class CommandParser {
	
public:
	CommandParser();
	~CommandParser();

	std::string splitString;

	std::string handleIDandCommand(int playerID, std::string command);
    Command getCommandFromString(std::string commandString);


private:
	//TODO World* worldID;
	int worldID;
};