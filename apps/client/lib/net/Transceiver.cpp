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

void Transceiver::writeWait( std::string message ) {
	this->writeToHost( message );
	this->readFromHost();
}

std::string Transceiver::read() {
//    std::cout << this->response.getBodyLength() << std::endl;
//	return this->response.getBodyString();
    return this->response.getBody();
}

// ------------------- PRIVATE ------------------

void Transceiver::connectToHost() {
	try {
		boost::asio::connect(
			this->connection->getSocket(),
			this->endpointIterator
		);
		
		std::cout << "Connected to server." << std::endl;
//		this->readFromHost();
		
	} catch ( std::exception& exception ) {
		std::cerr << exception.what() << std::endl;
		exit( EXIT_FAILURE );
	}
}


void Transceiver::readFromHost() {
    char buffer[512];
    
	boost::system::error_code error;
	std::size_t length = this->connection->getSocket().read_some(
		boost::asio::buffer( buffer ),
		error
	);
	if ( error ) { throw boost::system::system_error( error ); }
    
	this->response.saveBodyBuffer( buffer, length );
	
//    this->response.setBodyLength( length );
	std::cout << "- read once" << std::endl;
//    std::cout << this->response.getBody() << std::endl;
//    std::cout << "Data received: " << this->response.getBodyString() << std::endl;
}

void Transceiver::writeToHost( std::string message ) {
	boost::system::error_code error;
	boost::asio::write(
		this->connection->getSocket(),
		boost::asio::buffer( message ),
		error
	);
	
	if ( !error ) { 
		std::cout << "- write once" << std::endl;
//		this->readFromHost();
	}
//	this->readFromHost();
}