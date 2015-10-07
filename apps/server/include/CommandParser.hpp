#include <iostream>
#define COMMAND_SIZE 2

typedef struct {

	std::string type;
	std::string data;

}Command;

class CommandParser {
	
public:
	CommandParser();
	~CommandParser();


	Command getCommandFromString(std::string commandString);
	std::string[COMMAND_SIZE] parseFromString(std::string userCommand);


};