#include <iostream>
#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <unistd.h>

#include "Transceiver.hpp"
#include "NetConfig.hpp"

//attempt at a client with sockets

using boost::asio::ip::tcp;

int main() {
	std::shared_ptr< Transceiver > transceiver = std::make_shared< Transceiver >();
	transceiver->run();
	
	transceiver->write( "lg", "email@doge.do;password" );
	std::tuple< std::string, std::string > tuple = transceiver->read();
	std::cout << "Response received: " << std::endl;
	std::cout << "\tHeader: " << std::get< 0 >( tuple ) << std::endl;
	std::cout << "\tBody: " << std::get< 1 >( tuple ) << std::endl;
	
	transceiver->write( HEADER_LOGIN, "devon;test" );
	tuple = transceiver->read();
	std::cout << "Response received: " << std::endl;
	std::cout << "\tHeader: " << std::get< 0 >( tuple ) << std::endl;
	std::cout << "\tBody: " << std::get< 1 >( tuple ) << std::endl;
	
	transceiver->write( HEADER_COMMAND, "look north" );
	tuple = transceiver->read();
	std::cout << "Response received: " << std::endl;
	std::cout << "\tHeader: " << std::get< 0 >( tuple ) << std::endl;
	std::cout << "\tBody: " << std::get< 1 >( tuple ) << std::endl;
	
	
	
	
	transceiver->write( HEADER_LOGOUT, "might be hash in the future" );
	tuple = transceiver->read();
	std::cout << "Response received: " << std::endl;
	std::cout << "\tHeader: " << std::get< 0 >( tuple ) << std::endl;
	std::cout << "\tBody: " << std::get< 1 >( tuple ) << std::endl;
//	transceiver->write( "lgn", "email@doge.do;password" );
//	std::cout << transceiver->read() << std::endl;
	/*while (true) {
		transceiver->write( "lgn", "email@doge.do;password" );
	}*/
	
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