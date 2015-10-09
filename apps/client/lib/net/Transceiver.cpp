#include <iostream>
#include <functional>
#include <cstdlib>

#include "Transceiver.hpp"
#include "NetConfig.hpp"


using boost::asio::ip::tcp;


// ------------------- PUBLIC -------------------

Transceiver::Transceiver() {
	tcp::resolver resolver( ioService );
	tcp::resolver::query query( HOST_ADDRESS_IP4, std::to_string( HOST_PORT ) );
	endpointIterator = resolver.resolve( query );
	
	this->connection = std::make_shared< ServerConnection >( ioService, endpointIterator );
	this->connectToHost();
}


Transceiver::~Transceiver() {}


void Transceiver::run() {
	this->ioService.run();
}

void Transceiver::write( std::string message ) {
	this->writeToHost( message );
}

std::string Transceiver::read() {
	std::istream responseStream( &(this->response) );
	std::string message;
	std::getline( responseStream, message );
	
	return message;
}

// ------------------- PRIVATE ------------------

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
}




void Transceiver::readFromHost() {
	boost::system::error_code error;
	std::size_t responseLength = boost::asio::read_until(
		this->connection->getSocket(),
		this->response,
		'\n',
		error
	);
	
	if ( error ) { throw boost::system::system_error( error ); }
}

void Transceiver::writeToHost( std::string message ) {
	boost::system::error_code error;
	boost::asio::write(
		this->connection->getSocket(),
		boost::asio::buffer( message ),
		error
	);
	
	if ( error ) { throw boost::system::system_error( error ); }
}