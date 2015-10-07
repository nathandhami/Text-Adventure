#include <iostream>
#include <boost/array.hpp>
#include <boost/asio.hpp>

#include "Connector.hpp"

//attempt at a client with sockets

using boost::asio::ip::tcp;

int main() {
	Connector* connector = new Connector();
	connector->sendMessage( "Text" );
	std::cout << connector->getLastResponseFromHost() << std::endl;
	
	
	delete connector;

	std::string userName;
	std::string userPassword;
	std::string userLoginInfo;

	std::cout << "Text Gale Online " << std::endl << std::endl;
	std::cout << "Enter Username: ";
	std::getline(std::cin, userName);

	std::cout << "Enter Password: ";
	std::getline(std::cin, userPassword);
	
	userLoginInfo = userName + userPassword;
	std::cout << userLoginInfo << std::endl;

	//Sends the string to server to
	connector->sendMessage(userLoginInfo);

	//Parse server reponse here
	
	return 0;
}
