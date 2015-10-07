#include <iostream>
#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <unistd.h>

#include "Connector.hpp"
#include "ui_command.cpp"
#include "ui_world.cpp"

//attempt at a client with sockets

using boost::asio::ip::tcp;

int main() {
	Connector* connector = new Connector();
	connector->sendMessage( "Text" );
	std::cout << connector->getLastResponseFromHost() << std::endl;
	
	delete connector;

	std::string userName;
	std::string userPassword;
	std::string sendUserLoginInfo;

	std::cout << "Text Gale Online " << std::endl << std::endl;
	std::cout << "Enter Username: ";
	std::getline(std::cin, userName);

	std::cout << "Enter Password: ";
	std::getline(std::cin, userPassword);
	
	sendUserLoginInfo = "command:login;data:" + userName + "," + userPassword;

	//Sends this string to server:
	std::cout << sendUserLoginInfo;
	//connector->sendMessage(userLoginInfo);
	//Wait for server response
	sleep(1);

	//Parse server reponse here and database here
	std::cout << "\nSuccessfully logged in" << std::endl;

	sleep(1);

	int menuOption;

	std::cout << "Welcome, choose a menu item below (#)" << std::endl;
	std::cout << "1. Create Character" << std::endl;
	std::cout << "2. Exit" << std::endl;
	std::cin >> menuOption;

	switch(menuOption){
		case 1:
			std::cout << "Game start" << std::endl;
			break;
		case 2:
			break;
		default:
			std::cout << "Choice not valid, choose again" << std::endl;
			
	}

	//ui_command ui;
	//ui.readUserCommand();
		

	return 0;
}
