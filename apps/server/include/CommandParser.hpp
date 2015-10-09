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

	std::string splitString;

	Command getCommandFromString(std::string commandString);
	void checkCommandIsValid(Command commandToValidate);
	std::string getSplitString(std::string commandString);

private:
	char stringDelimiter = ';';
	char commandDelimiter = ':';
	char loginDelimiter = ',';
};