#include <iostream>
#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <unistd.h>

#include "Connector.hpp"
#include "ui_command.cpp"
#include "ui_world.cpp"

//attempt at a client with sockets

using boost::asio::ip::tcp;

void login()
{
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

	//Sends this string to server:
	std::cout << sendUserInfo << std::endl;
	//connector->sendMessage(userLoginInfo);
	//Wait for server response
	sleep(1);
}

int main() {
	
	login();

	int menuOption;
	std::string userCommand;
	bool isValid;

	ui_command ui;
	
	std::cout << "Please enter a menu item" << std::endl;

	/*do{
		std::cout << "1.Create character" << std::endl;
		std::cout << "2.Exit" << std::endl;
		std::cin >> menuOption;		
		
		switch(menuOption){
			case 1:
				std::cout << "Game start" << std::endl;
				isValid = true;
				break;
			case 2:
				isValid = true;
				break;
			default:
				std::cout << "Invalid menu item. Please choose again (#)." << std::endl;
				isValid = false;
				break;
				continue;
		}
	}while(!isValid);*/	
	
	std::cout << "Type 'h' for help and 'q' for quit" << std::endl;
	while(1){
		userCommand = ui.readUserCommand();
		std::cout << userCommand << std::endl;
	}

	return 0;
}
