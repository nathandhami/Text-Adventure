#include <iostream>
#define COMMAND_SIZE 2

typedef struct {

	std::string type;
	std::string data;
	std::string playerID;

}Command;

class CommandParser {
	
public:
	CommandParser();
	~CommandParser();


	Command getCommandFromString(std::string commandString);
	void checkCommandIsValid(Command commandToValidate);



};