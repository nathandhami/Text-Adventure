#include <iostream>
#include <functional>
#include <cstdlib>

#include "Transceiver.hpp"
#include "NetConfig.hpp"

/*
* Async lines are left for temporary reference
*/
using boost::asio::ip::tcp;


Transceiver::Transceiver() {
	tcp::resolver resolver( ioService );
	tcp::resolver::query query( HOST_ADDRESS_IP4, std::to_string( HOST_PORT ) );
	endpointIterator = resolver.resolve( query );
	
	this->connection = std::make_shared< ServerConnection >( ioService, endpointIterator );
	this->connectToHost();
}



Transceiver::~Transceiver() {}


void Transceiver::connectToHost() {
	try {
		boost::asio::connect(
			this->connection->getSocket(),
			this->endpointIterator
		);
		std::cout << "Connected to server." << std::endl;
		this->readFromHost();
		
	} catch ( std::exception& exception ) {
		std::cerr << exception.what() << std::endl;
		exit( EXIT_FAILURE );
	}
//	boost::asio::async_connect(
//		this->connection->getSocket(),
//		this->endpointIterator,
//		std::bind( &Transceiver::handleConnect, this, boost::system::error_code )
//	);
}




void Transceiver::readFromHost() {
	boost::system::error_code error;
	std::size_t responseLength = boost::asio::read_until(
		this->connection->getSocket(),
		this->response,
		'\n',
		error
	);
	
	if ( error ) { throw boost::system::system_error(error); }
	
	std::istream response_stream(&response);
	std::string status_message;
	std::getline(response_stream, status_message);
	std::cout << status_message << std::endl;
	
	//temp to test, will be replaced
//	boost::asio::async_read_until(
//		this->connection->getSocket(), 
//		this->response,
//		'\n',
//		[this](boost::system::error_code ec, std::size_t /*length*/){
//			std::istream response_stream(&response);
//			std::string status_message;
//			std::getline(response_stream, status_message);
//			std::cout << status_message << std::endl;
//		}
//	);
}

void Transceiver::writeToHost() {
	
}


void Transceiver::run() {
	this->ioService.run();
}