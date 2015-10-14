#include "Session.hpp"
#include "Authenticator.hpp"
#include "NetConfig.hpp"
#include "CommandParser.hpp"

#include <boost/asio/socket_base.hpp>

// Server-side messages
#define MESSAGE_CONNECT 	"A user has connected."
#define MESSAGE_DISCONNECT 	"A user has disconnected."

// Error messages (to client)
#define MESSAGE_ERROR_NOT_LOGGED_IN		"You are not logged in."
#define MESSAGE_ERROR_LOGGED_IN			"You have already logged in."
#define MESSAGE_ERROR_WRONG_CREDENTIALS	"Incorrect username or password."

// OK messages (to client)
//#define MESSAGE_OK_LOGGED_IN
#define MESSAGE_OK_LOGGED_OUT	"Log out successful."


using boost::asio::ip::tcp;


// ------------------- PUBLIC -------------------

tcp::socket& Session::getSocket() {
	return this->socket;
}


void Session::start() {
	// Initial connection established
	std::string address( this->getIP( Session::IPType::v4 ) );
	std::cout << MESSAGE_CONNECT << " IPv4: " << address << std::endl;
	// Start listening to the client
	this->readHeader();
}


// ------------------- PRIVATE ------------------

std::string Session::getIP( IPType type ) {
	return this->socket.remote_endpoint().address().to_string();
}


void Session::readHeader() {
	this->socket.async_read_some(
		boost::asio::buffer( this->bufferHeader, NetMessage::MaxLength::HEADER ),
		[ this ]( boost::system::error_code ec, std::size_t /*length*/ ) {
			if ( !ec ) {
				this->request.saveHeaderBuffer( this->bufferHeader );
				std::cout << "Received header: " << this->request.getHeader() << std::endl;
				this->readBody();
			} else {
				Authenticator::logout( this->userId );
				std::cout << MESSAGE_DISCONNECT << std::endl;
			}
		}
	);
}


void Session::readBody() {
	this->socket.async_read_some(
		boost::asio::buffer( this->bufferBody, NetMessage::MaxLength::BODY ),
		[ this ]( boost::system::error_code ec, std::size_t length ) {
			if ( !ec ) {
				this->request.saveBodyBuffer( this->bufferBody, length );
				std::cout << "Received body: " << this->request.getBody() << std::endl;
				this->handleRequest();
			} else {
				Authenticator::logout( this->userId );
				std::cout << MESSAGE_DISCONNECT << std::endl;
			}
		}
	);
}


void Session::handleRequest() {
	if ( this->request.getHeader() == HEADER_LOGIN ) {
		if ( this-> authorized ) {
			this->writeToClient( HEADER_ERROR, MESSAGE_ERROR_LOGGED_IN );
		}
		
		int userId = Authenticator::login( this->request.getBody() );
		if ( userId ) {
			this->authorized = true;
			this->userId = userId;
			this->writeToClient( HEADER_OK, "Log in successful." );
		} else {
			this->writeToClient( HEADER_ERROR, "Incorrect username or password." );
		}
		
	} else if ( this->request.getHeader() == HEADER_LOGOUT ) {
		if ( !( this->authorized ) ) {
			this->writeToClient( HEADER_ERROR, MESSAGE_ERROR_NOT_LOGGED_IN );
		}
		
		Authenticator::logout( this->userId );
		this->authorized = false;
		this->userId = 0;
		this->writeToClient( HEADER_OK, MESSAGE_OK_LOGGED_OUT );
	} else if ( this->request.getHeader() == HEADER_COMMAND ) {
		if ( !( this->authorized ) ) {
			this->writeToClient( HEADER_ERROR, MESSAGE_ERROR_NOT_LOGGED_IN );
		}
		
		std::string parserResponse = CommandParser::handleIDandCommand( this->userId, this->request.getBody() );
		if ( parserResponse == HEADER_ERROR ) {
			this->writeToClient( HEADER_ERROR, "Invalid Command." );
		} else {
			this->writeToClient( HEADER_OK, parserResponse );
		}
	} else {
		this->writeToClient( HEADER_ERROR, "Incorrect request." );
	}
}


bool Session::write( std::string message ) {
	boost::system::error_code error;
	boost::asio::write(
		this->socket,
		boost::asio::buffer( message ),
		error
	);
	if ( error ) { 
		Authenticator::logout( this->userId );
		std::cout << MESSAGE_DISCONNECT << std::endl;
		return false;
	}
	return true;
}


void Session::writeToClient( std::string header, std::string body ) {
	if ( !( this->write( header ) && this->write( body ) ) ) return;
	this->readHeader();
}
