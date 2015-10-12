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

	int menuOption;
	std::string userCommand;
	bool isValid;

	ui_command ui;

	ui.login();
	
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
