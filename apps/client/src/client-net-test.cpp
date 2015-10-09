#include <iostream>
#include <boost/array.hpp>
#include <boost/asio.hpp>

#include "Transceiver.hpp"

//attempt at a client with sockets

using boost::asio::ip::tcp;

int main() {
	Transceiver* connector = new Transceiver();
	connector->run();
	
	connector->write( "hello world\n" );
	std::cout << connector->read() << std::endl;
	
	
	delete connector;
	return 0;
}