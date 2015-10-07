//user interface for command
#include <iostream>

std::string readUserCommand ();
bool checkInputValidity( std::string userInput);
bool sendInput (std::string validInput);
void askForValidCommand();

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

std::string readUserCommand (){

	//read in command
	std::string command;
	std::cout << "Enter Command: ";
	std::getline(std::cin, command);
	return command;
}

bool checkInputValidity( std::string userInput){

	return true;
}

bool sendInput (std::string validInput){

	std::cout << validInput << " should be sent to server" << std::endl;
	return true;
}

void askForValidCommand(){
	
	std::cout << "user input is not valid. press h for help." << std::endl;
}