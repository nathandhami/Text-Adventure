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

void Transceiver::write( std::string header, std::string body ) {
	this->writeToHost( header );
	this->writeToHost( body );
	this->readHeaderFromHost();
	this->readBodyFromHost();
}

std::tuple< std::string, std::string > Transceiver::read() {
	return std::make_tuple( 
		this->response.getHeader(), 
		this->response.getBody() 
	);
}

// ------------------- PRIVATE ------------------

void Transceiver::connectToHost() {
	try {
		boost::asio::connect(
			this->connection->getSocket(),
			this->endpointIterator
		);
		
//		std::cout << "Connected to server." << std::endl;
//		this->readFromHost();
		
	} catch ( std::exception& exception ) {
		std::cerr << exception.what() << std::endl;
		exit( EXIT_FAILURE );
	}
}


void Transceiver::readHeaderFromHost() {
	char buffer[ NetMessage::MaxLength::HEADER ];
	boost::system::error_code error;
	
	this->connection->getSocket().read_some(
		boost::asio::buffer( buffer ),
		error
	);
	if ( error ) { 
		std::cerr << "Lost connection to server." << std::endl;
		exit( EXIT_FAILURE );
	}
	
	this->response.saveHeaderBuffer( buffer );
}


void Transceiver::readBodyFromHost() {
	char buffer[ NetMessage::MaxLength::BODY ];
    
	boost::system::error_code error;
	std::size_t length = this->connection->getSocket().read_some(
		boost::asio::buffer( buffer ),
		error
	);
	if ( error ) {
		std::cerr << "Lost connection to server." << std::endl;
		exit( EXIT_FAILURE );
//		throw boost::system::system_error( error ); 
	}
    
	this->response.saveBodyBuffer( buffer, length );
}


void Transceiver::writeToHost( std::string message ) {
	boost::system::error_code error;
	boost::asio::write(
		this->connection->getSocket(),
		boost::asio::buffer( message ),
		error
	);
	
	if ( error ) {
		std::cerr << "Failed to write to server" << std::endl;
		exit( EXIT_FAILURE );
	}
}
