#include "Session.hpp"
#include "Authenticator.hpp"
#include "NetConfig.hpp"
#include "CommandParser.hpp"

#include <future>
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
	this->readHeader(); // v
	// this->asyncReadUserRequest();
}


// ------------------- PRIVATE ------------------

std::string Session::getIP( IPType type ) {
	return this->socket.remote_endpoint().address().to_string();
}


// new function for 'better' async reading
void Session::asyncReadUserRequest() {
	std::async(
		std::launch::async,
		[ this ]() {
			// while user connected
			// string = do read header;
			// string = do read body;
			// handleRequest( string, string )
		}
	);
}


// new read header function for improved async
std::string Session::readHeader() {
	std::cout << "Waiting for client header..." << std::endl;
	// do blocking stuff
}


// new read body function for improved async
std::string Session::readBody() {
	std::cout << "Waiting for client body..." << std::endl;
	// do blocking stuff
}


// new handle request function for improved async
void Session::handleRequest( const std::string header, const std::string body ) {
	std::cout << "Processing client's request..." << std::endl;
	// do blocking stuff
	// possible create a map with pointers to parsing functions
}


/*void Session::readHeader() {
	std::cout << "Waiting for head" << std::endl;
	this->socket.async_read_some(
		boost::asio::buffer( this->bufferHeader, NetMessage::MaxLength::HEADER ),
		[ this ]( boost::system::error_code ec, std::size_t length ) {
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
}*/


/*void Session::readBody() {
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
}*/


/*void Session::handleRequest() {
	if ( this->request.getHeader() == HEADER_LOGIN ) { //LOGIN
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
		
	} else if ( this->request.getHeader() == HEADER_LOGOUT ) { //LOG OUT
		if ( !( this->authorized ) ) {
			this->writeToClient( HEADER_ERROR, MESSAGE_ERROR_NOT_LOGGED_IN );
		}
		
		Authenticator::logout( this->userId );
		this->authorized = false;
		this->userId = 0;
		this->writeToClient( HEADER_OK, MESSAGE_OK_LOGGED_OUT );
	} else if ( this->request.getHeader() == HEADER_COMMAND ) { //ACTION
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
	this->readHeader();
}*/



//bool Session::write( std::string message ) {
//	boost::asio::async_write(
//		this->socket,
//		boost::asio::buffer( message, message.length() ),
//		[ this ]( boost::system::error_code ec, std::size_t /*length*/ ) {
//			if ( !ec ) {
//				std::cout << "Wrote stuff." << std::endl;
//				this->write( "Login Succ." );
//			}
//		}
//	);
//}


bool Session::write( std::string message ) {
	std::cout << "Write called: " << message << std::endl;
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


/*void Session::writeToClient( std::string header, std::string body ) {
//	if ( !( this->write( header ) && this->write( body ) ) ) return;
	this->write( header );
//	this->readHeader();
}*/


void Session::asyncWrite() {
	std::async(
		std::launch::async,
		[ this ]() {
			
			this->messageQueueLock.lock();
			NetMessage message = this->responseMessageQueue.front();
			this->responseMessageQueue.pop();
			this->messageQueueLock.unlock();
			
			this->write( message.getHeader() );
			this->write( message.getBody() );
			
			if ( !(this->responseMessageQueue.empty()) ) {
				this->asyncWrite();
			} else {
				this->writeInProgress = false;
			}
		}
	);
}


void Session::writeToClient( std::string header, std::string body ) {
	NetMessage responseMessage;
	responseMessage.saveHeaderString( header );
	responseMessage.saveBodyString( body );
	
	this->messageQueueLock.lock();
	this->responseMessageQueue.push( responseMessage );
	this->messageQueueLock.unlock();
	
	if ( !writeInProgress ) {
		this->writeInProgress = true;
		this->asyncWrite();
	}
}


































