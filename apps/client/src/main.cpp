#include <iostream>
#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <unistd.h>

#include "Transceiver.hpp"
#include "ui_command.cpp"
#include "ui_world.cpp"
#include "NetConfig.hpp"

//attempt at a client with sockets

using boost::asio::ip::tcp;
int hi(std::string token, ui_command ui);
std::string readCommand(ui_command ui);
int main() {

	int menuOption;
	std::string userCommand;
	bool isValid;

	ui_command ui;

	std::string token = ui.login();

	hi(token, ui);
	
	//std::cout << "Please enter a menu item" << std::endl;

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
	
	//std::cout << "Type 'h' for help and 'q' for quit" << std::endl;
	//while(1){
	//	userCommand = ui.readUserCommand();
	//	std::cout << userCommand << std::endl;
	//}

	return 0;
}

std::string readCommand(ui_command ui)
{
	std::string userCommand = ui.readUserCommand();
	return userCommand;
}
/*
#include <iostream>
#include <boost/array.hpp>
#include <boost/asio.hpp>

#include "Transceiver.hpp"
#include "NetConfig.hpp"

//attempt at a client with sockets

using boost::asio::ip::tcp;
*/
int hi(std::string token, ui_command ui) {
	std::shared_ptr< Transceiver > transceiver = std::make_shared< Transceiver >();
	transceiver->run();
	
	transceiver->write( "lg", token );
	std::tuple< std::string, std::string > tuple = transceiver->read();
	std::cout << "Response received: " << std::endl;
	std::cout << "\tHeader: " << std::get< 0 >( tuple ) << std::endl;
	std::cout << "\tBody: " << std::get< 1 >( tuple ) << std::endl;
	//write( header_login, id;pw)
	transceiver->write( HEADER_LOGIN, token );
	tuple = transceiver->read();
	std::cout << "Response received: " << std::endl;
	std::cout << "\tHeader: " << std::get< 0 >( tuple ) << std::endl;
	std::cout << "\tBody: " << std::get< 1 >( tuple ) << std::endl;
	
	std::string command = readCommand(ui);
	//write(HEADER_COMMAND, "wat;)
	while( command != "quit")
	{
		transceiver->write( HEADER_COMMAND, command );
		tuple = transceiver->read();
		std::cout << "Response received: " << std::endl;
		std::cout << "\tHeader: " << std::get< 0 >( tuple ) << std::endl;
		std::cout << "\tBody: " << std::get< 1 >( tuple ) << std::endl;
		command = readCommand(ui);
	}

	transceiver->write( HEADER_LOGOUT, "might be hash in the future" );
	tuple = transceiver->read();
	std::cout << "Response received: " << std::endl;
	std::cout << "\tHeader: " << std::get< 0 >( tuple ) << std::endl;
	std::cout << "\tBody: " << std::get< 1 >( tuple ) << std::endl;
//	transceiver->write( "lgn", "email@doge.do;password" );
//	std::cout << transceiver->read() << std::endl;
	/*while (true) {
		transceiver->write( "lgn", "email@doge.do;password" );
	}
	*/
//	std::cout << connector->read() << std::endl;
//
//	connector->write( "hello woooolsta\n" );
//	std::cout << connector->read() << std::endl;
//	connector->write( "hello woooolstar\n" );
//	std::cout << connector->read() << std::endl;
//	connector->write( "hello woooorlsta\n" );
//	std::cout << connector->read() << std::endl;
	
	
	
	return 0;
}

