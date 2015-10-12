//user interface for command
#include <iostream>
#include <boost/tokenizer.hpp>
#include <boost/algorithm/string.hpp> 

using namespace boost;

class ui_command
{
public:
	void login();	

	std::string readUserCommand ();

	bool checkInputValidity( std::string userInput);

	bool sendInput (std::string validInput);
	
	void askForValidCommand();
};

void ui_command::login(){
	std::string userName;
	std::string userPassword;
	std::string sendUserInfo;
	bool isUserEmpty = true;
	bool isPassEmpty = true;

	std::cout << "\nText Gale Online " << std::endl << std::endl;

	while(isUserEmpty)
	{
		std::cout << "Enter Username: ";
		std::getline(std::cin, userName);
		isUserEmpty = false;

		if(userName.length() < 1)
		{
			isUserEmpty = true;
			std::cout << "Username cannot be empty, please try again" << std::endl;
			continue;
		}
	}

	while(isPassEmpty)
	{
		std::cout << "Enter Password: ";
		std::getline(std::cin, userPassword);
		isPassEmpty = false;

		if(userPassword.length() < 1)
		{
			isPassEmpty = true;
			std::cout << "Password cannot be empty, please try again" << std::endl;
			continue;
		}
	}
	
	sendUserInfo = "command:login;data:" + userName + "," + userPassword + "\n";

	//Sends this string to server here
	std::cout << sendUserInfo << std::endl;
	//Wait for server response here
	sleep(1);
}

std::string ui_command::readUserCommand (){

	//read in command
	std::string command;
	std::cout << "Enter Command: ";
	std::getline(std::cin, command);

	std::string commandString[1024];
	int i = 0;

	tokenizer<> tok(command);
   	for(tokenizer<>::iterator beg=tok.begin(); beg!=tok.end();++beg){
		commandString[i] = *beg;
		++i;
   	}

	to_lower(commandString[0]);
	command = "command:" + commandString[0] + ";" + "data:" + commandString[1];
	
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
