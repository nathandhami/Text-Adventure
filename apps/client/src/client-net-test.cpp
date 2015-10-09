#include <iostream>
#include <boost/array.hpp>
#include <boost/asio.hpp>

#include "Transceiver.hpp"

//attempt at a client with sockets

using boost::asio::ip::tcp;

int main() {
	Transceiver* connector = new Transceiver();
	connector->run();
	
	connector->write( "lg" );
	connector->writeWait( "email@email.doge" );
	std::cout << connector->read() << std::endl;
	
	connector->write( "lgn" );
	connector->writeWait( "email@email.doge" );
	std::cout << connector->read() << std::endl;
	
	
//	std::cout << connector->read() << std::endl;
//
//	connector->write( "hello woooolsta\n" );
//	std::cout << connector->read() << std::endl;
//	connector->write( "hello woooolstar\n" );
//	std::cout << connector->read() << std::endl;
//	connector->write( "hello woooorlsta\n" );
//	std::cout << connector->read() << std::endl;
	
	
	delete connector;
	return 0;
}