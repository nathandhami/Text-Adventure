#include "Session.hpp"
#include "Authenticator.hpp"
#include "NetConfig.hpp"
#include "GameCode.hpp"
#include "CommandParser.hpp"
#include "Server.hpp"
#include "CarrierPigeon.hpp"

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

// Useful Macros
#define LOG( msg ) std::cout << "[" << this->identifierString << "] " << msg << std::endl


using boost::asio::ip::tcp;


// ------------------- PUBLIC -------------------

Session::~Session() {
	LOG( "--- Process terminated ---" );
}

tcp::socket& Session::getSocket() {
	return this->socket;
}


void Session::start( std::string identifierString ) {
	// Initial connection established
	std::string address( this->getIP( Session::IPType::v4 ) );
	this->identifierString = identifierString;
	
	LOG( "Connected: " << address );
	
	this->asyncReadUserRequest();
	std::cout << "NOW ASYNC" << std::endl;
	this->readerThread.join();
}


void Session::writeToClient( std::string header, std::string body ) {
	NetMessage responseMessage( header, body );

	this->messageQueueLock.lock();
	this->responseMessageQueue.push( responseMessage );
	this->messageQueueLock.unlock();

	if ( !this->writing ) {
		this->writing = true;
		this->asyncWrite();
	}
}


// ------------------- PRIVATE ------------------

std::string Session::getIP( IPType type ) {
	return this->socket.remote_endpoint().address().to_string();
}


// new function for 'better' async reading
// TO-DO: do a proper session shutdown if user dcs
void Session::asyncReadUserRequest() {
	this->readerThread = std::thread(
		[ this ]() {
			this->reading = true;
			while ( this->reading ) {
				// while user connected
				// get header
				std::string header = this->read( NetMessage::MaxLength::HEADER );
				if ( header == CODE_ERROR_READ ) {
					this->gentleShutDown();
					break;
				}
				// get body length
				std::string bodyLengthStr = this->read( NetMessage::MaxLength::BODY_LENGTH );
				if ( header == CODE_ERROR_READ ) {
					this->gentleShutDown();
					break;
				}
				int bodyLength = atoi( bodyLengthStr.c_str() );
				// get body
				std::string body = this->read( bodyLength );
				if ( header == CODE_ERROR_READ ) {
					this->gentleShutDown();
					break;
				}
				
				LOG( "Received request." );
				
				this->handleRequest( header, body );
			}
			LOG( "Stopped reading." );
			this->terminating = true;
		}
	);
}


// new read function for the improved async
std::string Session::read( const int maxBufferLength ) {
//	std::cout << "Waiting for client write..." << std::endl;
	
	std::vector< char > buffer( maxBufferLength );
	boost::system::error_code error;
	
	size_t bufferLength = boost::asio::read(
		this->socket,
		boost::asio::buffer( buffer ),
		error
	);
	
	if ( error ) {
		return CODE_ERROR_READ;
	}
//	std::cout << "\tGOT: " <<  std::string( buffer.begin(), buffer.begin() + bufferLength ) << std::endl;
	
	return std::string( buffer.begin(), buffer.begin() + bufferLength );
}


// new handle request function for improved async
void Session::handleRequest( const std::string header, const std::string body ) {
	LOG( "Processing client's request..." );
	LOG( "> Header: " << header );
	LOG( "> Body: " << body );
	
	Session::ExecFuncMap::const_iterator iterator = this->funcMap.find( header );
	if ( iterator == funcMap.end() ) {
		// not found
		this->writeToClient( HEADER_ERROR, "Server error: incorrect request." );
		return;
	}
	Session::ExecuteFunction func = iterator->second;
	( this->*func )( body );
}


// ------------- De-headed functions

void Session::login( const std::string& credentials ) {
	LOG( "Login happened." );
	
	bool loginSuccess = Authenticator::login( this->currentUser, credentials );
	if ( !loginSuccess ) {
		this->writeToClient( GameCode::WRONG, MESSAGE_ERROR_WRONG_CREDENTIALS );
		return;
	}
	
	std::cout << "Login success." << std::endl;
	if ( DatabaseTool::isCharOnline( this->currentUser.getUserId() ) ) {
		this->writeToClient( GameCode::INVALID, "Already logged in." );
		return;
	}
	DatabaseTool::setCharOnline( this->currentUser.getUserId(), this->identifierString );
	this->writeToClient( GameCode::CORRECT, "a list\nof various\ncharacters" );
}

void Session::logout( const std::string& credentials ) {
	LOG( "Logout happened." );
	
	bool logoutSuccess = Authenticator::logout( this->currentUser );
	if ( !logoutSuccess ) {
		this->writeToClient( HEADER_ERROR, "Could not log out." );
		return;
	}
	
	this->writeToClient( HEADER_OK, MESSAGE_OK_LOGGED_OUT );
}


// Movement, observation, combat, chat, interaction
void Session::doGameCommand( const std::string& commandString ) {
	
	/*
	
	int header ::= parser::getheader
	
	if header = WORLDHEADER
		world::dostuff command
	else if header = MESSAGEHEADER
		messenger::dostuff command
	else if header = COMBAtHEADER
		combater::dostuff command
	else
		write invalid
	
	*/
	
	LOG( "Command happened." );
	Server::sendMessageToCharacter( this->currentUser.getUserId(), GameCode::STATUS, "some random stuff" );
	CarrierPigeon::deliverPackage( 1 );
	
//	std::cout << "Command happened." << std::endl;
	std::string parserResponse = CommandParser::handleIDandCommand( this->currentUser.getUserId(), commandString );
	if ( parserResponse == HEADER_ERROR ) {
		this->writeToClient( HEADER_ERROR, "Invalid Command." );
	} else {
		this->writeToClient( HEADER_OK, parserResponse );
	}
}

void Session::sendMessageToCharacter( const std::string& charNameAndMessage ) {
	// split char id and message
//	Server::sendMessageToClient( "sdf-dsfsd-dsfs", charNameAndMessage );
}

// ------------ End 

void Session::asyncWrite() {
	this->writerThread = std::thread(
		[ this ]() {
			while ( this->writing ) {
				this->messageQueueLock.lock();
				// ----- critical section -----
				NetMessage message = this->responseMessageQueue.front();
				this->responseMessageQueue.pop();
				// --- end critical section ---
				this->messageQueueLock.unlock();
				
//				this->write( message.header );
//				this->write( message.body );
				
				
				// put size of the body into a string
				std::stringstream formatStream;
				formatStream << std::setfill( '0' ) << std::setw( NetMessage::MaxLength::BODY_LENGTH ) << message.body.length();

				// write to server ensuring no disconnects
				if ( !this->write( message.header ) ) {
					LOG( "Can't wite: client disconnected." );
//					std::cout << "Write Failed." << std::endl;
//					this->gentleShutDown();
				} else if ( !this->write( formatStream.str() ) ) {
					LOG( "Can't wite: client disconnected." );
//					std::cout << "Write Failed." << std::endl;
//					this->gentleShutDown();
				} else if ( !this->write( message.body ) ) {
					LOG( "Can't wite: client disconnected." );
//					std::cout << "Write Failed." << std::endl;
//					this->gentleShutDown();
				}
				
				
				
				if ( this->responseMessageQueue.empty() ) {
					this->writing = false;
					this->writerThread.detach();
				}
//				std::cout << "Done writing." << std::endl;
			}
			LOG( "Stopped writing." );
		}
	);
}


bool Session::write( std::string message ) {
//	std::cout << "Write called: " << message << std::endl;
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


void Session::gentleShutDown() {
	
	LOG( "The client has disconnected. Terminating..." );
	this->writing = false;
	this->reading = false;
	
	LOG( "SESSION SAFELY ENDED." );
	
}































