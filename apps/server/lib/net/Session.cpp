#include "Session.hpp"
#include "Authenticator.hpp"
#include "NetConfig.hpp"
#include "CommandParser.hpp"
#include "Server.hpp"

#include <future>
#include <boost/asio/socket_base.hpp>

// Error read/write string codes
#define CODE_ERROR_READ		"rerr"
#define CODE_ERROR_WRITE	"werr"

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
	
	// Initialize the user
	this->currentUser.userId = 0;
	this->currentUser.authorized = false;
	this->currentUser.characterId = 0;
	
	// Start listening to the client
//	this->readHeader(); // v
	this->asyncReadUserRequest();
}


void Session::writeToClient( std::string header, std::string body ) {
	NetMessage responseMessage( header, body );
//	responseMessage.saveHeaderString( header );
//	responseMessage.saveBodyString( body );

	this->messageQueueLock.lock();
	this->responseMessageQueue.push( responseMessage );
	this->messageQueueLock.unlock();

	if ( !writeInProgress ) {
		this->writeInProgress = true;
		this->asyncWrite();
	}
}


// ------------------- PRIVATE ------------------

std::string Session::getIP( IPType type ) {
	return this->socket.remote_endpoint().address().to_string();
}


// new function for 'better' async reading
void Session::asyncReadUserRequest() {
	this->readerThread = std::thread(
		[ this ]() {
			bool running = true;
			while ( running ) {
				// while user connected
				// string = do read header;
				std::string header = this->read( NetMessage::MaxLength::HEADER );
				// string = do read body;
				std::string body = this->read( NetMessage::MaxLength::BODY );
				
				if ( header == CODE_ERROR_READ || body == CODE_ERROR_READ ) {
					// handle user disconnect
					running = false;
				} else {
					this->handleRequest( header, body );
				}
			}
		}
	);
}


// new read function for the improved async
std::string Session::read( const int maxBufferLength ) {
	std::cout << "Waiting for client write..." << std::endl;
	
	std::vector< char > buffer( maxBufferLength );
	boost::system::error_code error;
	
	size_t bufferLength = this->socket.read_some(
		boost::asio::buffer( buffer ),
		error
	);
	
	if ( error ) {
		return CODE_ERROR_READ;
	}
	
	return std::string( buffer.begin(), buffer.begin() + bufferLength );
}


// new handle request function for improved async
void Session::handleRequest( const std::string header, const std::string body ) {
	std::cout << "Processing client's request..." << std::endl;
	
	Session::ExecFuncMap::const_iterator iterator = this->funcMap.find( header );
	if ( iterator == funcMap.end() ) {
		// not found
		this->writeToClient( HEADER_ERROR, "Server error: incorrect request." );
	}
	Session::ExecuteFunction func = iterator->second;
	( this->*func )( body );
}


// ------------- De-headed functions

void Session::login( const std::string& credentials ) {
	std::cout << "Login happened." << std::endl;
	this->currentUser.userId = Authenticator::login( credentials );
	if ( !this->currentUser.userId ) {
		this->writeToClient( HEADER_ERROR, MESSAGE_ERROR_WRONG_CREDENTIALS );
	} else {
		std::cout << "Login success." << std::endl;
		this->currentUser.authorized = true;
		this->writeToClient( HEADER_OK, "a list\nof various\ncharacters" );
	}
}

void Session::logout( const std::string& credentials ) {
	std::cout << "Logout happened." << std::endl;
	Authenticator::logout( this->currentUser.userId );
	this->currentUser.userId = 0;
	this->currentUser.authorized = false;
	this->currentUser.characterId = 0;
	this->writeToClient( HEADER_OK, MESSAGE_OK_LOGGED_OUT );

}

void Session::doGameCommand( const std::string& commandString ) {
	std::cout << "Command happened." << std::endl;
	std::string parserResponse = CommandParser::handleIDandCommand( this->currentUser.userId, commandString );
	if ( parserResponse == HEADER_ERROR ) {
		this->writeToClient( HEADER_ERROR, "Invalid Command." );
	} else {
		this->writeToClient( HEADER_OK, parserResponse );
	}
}

void Session::sendMessageToCharacter( const std::string& charNameAndMessage ) {
	// split char id and message
	Server::sendMessageToClient( "sdf-dsfsd-dsfs", charNameAndMessage );
}

// ------------ End 

void Session::asyncWrite() {
	this->writerThread = std::thread(
		[ this ]() {
			while ( this->writeInProgress ) {
				this->messageQueueLock.lock();
				NetMessage message = this->responseMessageQueue.front();
				this->responseMessageQueue.pop();
				this->messageQueueLock.unlock();
				
				this->write( message.header );
				this->write( message.body );
				
				if ( this->responseMessageQueue.empty() ) {
					this->writeInProgress = false;
					this->writerThread.detach();
				}
				std::cout << "Done writing." << std::endl;
			}
		}
	);
}


bool Session::write( std::string message ) {
	std::cout << "Write called: " << message << std::endl;
	boost::system::error_code error;
	boost::asio::write(
		this->socket,
		boost::asio::buffer( message ),
		error
	);
	if ( error ) { 
		//		Authenticator::logout( this->userId );
		std::cout << MESSAGE_DISCONNECT << std::endl;
		return false;
	}
	return true;
}


































