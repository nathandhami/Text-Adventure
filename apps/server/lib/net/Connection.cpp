#include "Connection.hpp"

#include <boost/asio/socket_base.hpp>

using boost::asio::ip::tcp;


// ------------------- PUBLIC -------------------

tcp::socket& Connection::getSocket() {
	return this->socket;
}


void Connection::write( std::string message ) {
	boost::system::error_code error;
	boost::asio::write(
		this->socket,
		boost::asio::buffer( message ),
		error
	);
	if ( !error ) { 
		this->read();
	}
}


// ------------------- PRIVATE ------------------

void Connection::start() {
	// Initial connection established
	std::string address( this->getIP( Connection::IPType::v4 ) );
	std::cout << "User Connected." << std::endl;
	// Start listening to the client
	this->read();
}


std::string Connection::getIP( IPType type ) {
	return this->socket.remote_endpoint().address().to_string();
}


void Connection::readHeader() {
	char buffer[ NetMessage::MaxLength::HEADER ];
	boost::system::error_code error;
	
	std::size_t length = this->socket.read_some(
		
	);
}


void Connection::read() {
	char buffer[512];
	
	boost::system::error_code error;
	size_t length = this->socket.read_some(
		boost::asio::buffer( buffer ),
		error
	);
	if ( !error ) { 
		this->request.saveBodyBuffer( buffer, length );
//		std::cout
		this->write( this->request.getBody() );
	} else {
		//TO-DO inform database the user is offline
		std::cout << "User Disconnected." << std::endl;
	}
}