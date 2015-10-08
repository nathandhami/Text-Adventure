#include <iostream>

#include "Connector.hpp"
#include "NetConfig.hpp"


using boost::asio::ip::tcp;


Connector::Connector() {
	tcp::resolver resolver( this->ioService );
	tcp::resolver::query query( HOST_ADDRESS_IP4, std::to_string( HOST_PORT ) );
	this->endpointIterator = resolver.resolve( query );
}


Connector::~Connector() {
	
}


bool Connector::sendMessage( std::string message ) {
	try {
		this->openConnectionToHost();
		while ( true ) {
			boost::array< char, 128 > buf;
			boost::system::error_code error;
			
			size_t responseLength = this->socket->read_some( boost::asio::buffer( buf ), error );
			
			if ( error == boost::asio::error::eof ) {
				delete this->socket;
				return true;
			} else if ( error ) {
				throw boost::system::system_error( error );
			}
			this->lastResponse = std::string( buf.data(), responseLength );
		}
	} catch ( std::exception& exception ) {
		std::cerr << exception.what() << std::endl;
		return false;
	}
}

std::string Connector::getLastResponseFromHost() {
	return this->lastResponse;
}




void Connector::openConnectionToHost() {
	this->socket = new tcp::socket( this->ioService );
	boost::asio::connect( *(this->socket), this->endpointIterator );
}