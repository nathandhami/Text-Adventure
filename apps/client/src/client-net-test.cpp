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
	return 0;
}