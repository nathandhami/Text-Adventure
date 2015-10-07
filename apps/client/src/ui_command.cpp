//user interface for command
#include <iostream>

class ui_command
{
public:

	std::string readUserCommand ();

	bool checkInputValidity( std::string userInput);

	bool sendInput (std::string validInput);
	
	void askForValidCommand();
}

/*
int main(){

	std::string input;
	input = readUserCommand();
	if(checkInputValidity(input))
	{
		sendInput(input);
	}
	else
	{
		askForValidCommand();
	}
	return 0;
}
*/

std::string ui_command::readUserCommand (){

	//read in command
	std::string command;
	std::cout << "Enter Command: ";
	std::getline(std::cin, command);
	return command;
}

bool ui_command::checkInputValidity( std::string userInput){

	return true;
}

bool ui_command::sendInput (std::string validInput){

	std::cout << validInput << " should be sent to server" << std::endl;
	return true;
}

void ui_command::askForValidCommand(){

	std::cout << "user input is not valid. press h for help." << std::endl;
}
