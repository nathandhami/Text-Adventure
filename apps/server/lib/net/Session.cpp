#include "Session.hpp"
#include "Authenticator.hpp"
#include "NetConfig.hpp"

#include <boost/asio/socket_base.hpp>

using boost::asio::ip::tcp;


// ------------------- PUBLIC -------------------

tcp::socket& Session::getSocket() {
	return this->socket;
}


void Session::start() {
	// Initial connection established
	std::string address( this->getIP( Session::IPType::v4 ) );
	std::cout << "User Connected." << std::endl;
	// Start listening to the client
	this->readHeader();
}


// ------------------- PRIVATE ------------------

std::string Session::getIP( IPType type ) {
	return this->socket.remote_endpoint().address().to_string();
}


void Session::readHeader() {
	char buffer[ NetMessage::MaxLength::HEADER ];
	boost::system::error_code error;
	
	std::size_t length = this->socket.read_some(
		boost::asio::buffer( buffer ),
		error
	);
	if ( !error ) { 
		this->request.saveHeaderBuffer( buffer );
		std::cout << "Received header: " << this->request.getHeader() << std::endl;
		this->readBody();
	} else {
		//TO-DO inform database the user is offline
		std::cout << "User Disconnected." << std::endl;
	}
	
}


void Session::readBody() {
	char buffer[512];
	
	boost::system::error_code error;
	size_t length = this->socket.read_some(
		boost::asio::buffer( buffer ),
		error
	);
	if ( !error ) { 
		this->request.saveBodyBuffer( buffer, length );
		std::cout << "Received body: " << this->request.getBody() << std::endl;
		this->handleRequest();
	} else {
		//TO-DO inform database the user is offline
		std::cout << "User Disconnected." << std::endl;
	}
}


void Session::handleRequest() {
	if ( this->request.getHeader() == HEADER_LOGIN ) {
		int userId = Authenticator::login( this->request.getBody() );
		if ( userId ) {
			this->writeToClient( HEADER_OK, "Log in successful." );
		} else {
			this->writeToClient( HEADER_ERROR, "Incorrect username or password." );
		}
		
	} else {
		this->writeToClient( HEADER_ERROR, "Incorrect command." );
	}
}


void Session::write( std::string message ) {
	boost::system::error_code error;
	boost::asio::write(
		this->socket,
		boost::asio::buffer( message ),
		error
	);
	if ( !error ) { 
		//		this->readHeader();
	}
}


void Session::writeToClient( std::string header, std::string body ) {
	this->write( header );
	this->write( body );
	this->readHeader();
}
