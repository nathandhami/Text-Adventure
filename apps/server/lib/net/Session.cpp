#include "Session.hpp"
#include "Authenticator.hpp"
#include "NetConfig.hpp"
#include "GameCode.hpp"
#include "CommandParser.hpp"
#include "DictionaryCmds.hpp"
#include "Server.hpp"
#include "CarrierPigeon.hpp"
#include <mod/Editor.hpp>
#include <cmd/Commander.hpp>
#include <char/CharacterManager.hpp>
#include <char/Character.hpp>

#include <future>
#include <boost/asio/socket_base.hpp>
#include <chrono>

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
void Session::asyncReadUserRequest() {
	this->readerThread = std::thread(
		[ this ]() {
			using namespace std::chrono;
			
			const int REQUEST_THRESHOLD = 10;
			const int RELOAD_TIME_MS = 1000;
			const int READER_SLEEP_TIME_MS = 10;
			
			int requestCounter = 0;
			int nextReloadTime_ms = duration_cast< milliseconds >( system_clock::now().time_since_epoch() ).count() + RELOAD_TIME_MS;
			this->reading = true;
			
			while ( this->reading ) {
				// while user connected
				int currentTime_ms = duration_cast< milliseconds >( system_clock::now().time_since_epoch() ).count();
				LOG( "CTime: " << currentTime_ms );
				LOG( "Next reload time: " << nextReloadTime_ms );
					
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
				
				
				if ( nextReloadTime_ms <= currentTime_ms ) {
					requestCounter = 0;
					nextReloadTime_ms = currentTime_ms + RELOAD_TIME_MS;
				}
				
				if ( requestCounter >= REQUEST_THRESHOLD ) {
					this->writeToClient( GameCode::ERROR, "Server error: too many requests!" );
				} else {
					LOG( "Received request." );
					this->handleRequest( header, body );
					requestCounter++;
				}
				std::this_thread::sleep_for(std::chrono::milliseconds( READER_SLEEP_TIME_MS ));
			}
			LOG( "Stopped reading." );
			this->terminating = true;
		}
	);
}


// new read function for the improved async
std::string Session::read( const int maxBufferLength ) {
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


void Session::registerUser( const std::string& credentials ) {
	LOG( "Register happened" );
	
	std::pair< std::string, std::string > regResult = Authenticator::registerAccount( credentials );
	this->writeToClient( regResult.first, regResult.second );
}


void Session::login( const std::string& credentials ) {
	LOG( "Login happened." );
	
	bool loginSuccess = Authenticator::login( this->currentUser, credentials );
	if ( !loginSuccess ) {
		this->writeToClient( GameCode::WRONG, MESSAGE_ERROR_WRONG_CREDENTIALS );
		return;
	}
	
	std::cout << "Login success." << std::endl;
	
	this->writeToClient( GameCode::CORRECT, CharacterManager::getCharacterList( this->currentUser.getUserId() ) );
}

void Session::logout( const std::string& credentials ) {
	LOG( "Logout happened." );
	
	bool logoutSuccess = Authenticator::logout( this->currentUser );
	if ( !logoutSuccess ) {
		this->writeToClient( HEADER_ERROR, "Could not log out." );
		return;
	}
	
	this->writeToClient( GameCode::LOGOUT, MESSAGE_OK_LOGGED_OUT );
}


void Session::createCharacter( const std::string& charData ) {
	LOG( "Char-create happened." );
	
	std::pair< std::string, std::string > createResponse = CharacterManager::createCharacter( this->currentUser.getUserId(), charData );
	this->writeToClient( createResponse.first, createResponse.second );
}


void Session::deleteCharacter( const std::string& charName ) {
	LOG( "Char-delete happened." );

	std::pair< std::string, std::string > createResponse = CharacterManager::deleteCharacter( this->currentUser.getUserId(), charName );
	this->writeToClient( createResponse.first, createResponse.second );
}


void Session::selectCharacter( const std::string& charName ) {
	LOG( "Char-select happened." );
	
	if ( CharacterManager::selectCharacter( this->currentUser, charName, this->identifierString ) ) {
		this->writeToClient( GameCode::OK, "Character " + charName + " selected." );
		this->writeToClient( GameCode::ATTRIBUTES, Character::getStats( this->currentUser.getSelectedCharacterId() ) );
		this->writeToClient( GameCode::INVENTORY, Character::getInventory( this->currentUser.getSelectedCharacterId() ) );
	} else {
		this->writeToClient( GameCode::ERROR, "Could not select " + charName + ", internal server error occurred." );
	}
}


void Session::deselectCurrentCharacter( const std::string& placeholder ) {
	LOG( "Char-deselect happened." );
	
	CharacterManager::deselectCurrentCharacter( this->currentUser );
	this->writeToClient( GameCode::CHAR_DELECT, CharacterManager::getCharacterList( this->currentUser.getUserId() ) );
}


// Movement, observation, combat, chat, interaction
void Session::doGameCommand( const std::string& commandString ) {
	std::pair< std::string, std::string > commandResponse = Commander::handleCommand( this->currentUser, commandString );
	this->writeToClient( commandResponse.first, commandResponse.second );
}


// ------------ End of MAPPED functions

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
				
				// put size of the body into a string
				std::stringstream formatStream;
				formatStream << std::setfill( '0' ) << std::setw( NetMessage::MaxLength::BODY_LENGTH ) << message.body.length();

				// write to server ensuring no disconnects
				if ( !this->write( message.header ) ) {
					LOG( "Can't wite: client disconnected." );
				} else if ( !this->write( formatStream.str() ) ) {
					LOG( "Can't wite: client disconnected." );
				} else if ( !this->write( message.body ) ) {
					LOG( "Can't wite: client disconnected." );
				}
				
				if ( this->responseMessageQueue.empty() ) {
					this->writing = false;
					this->writerThread.detach();
				}
			}
			LOG( "Stopped writing." );
		}
	);
}


bool Session::write( std::string message ) {
	boost::system::error_code error;
	boost::asio::write(
		this->socket,
		boost::asio::buffer( message ),
		error
	);
	
	return !error;
}


void Session::gentleShutDown() {
	LOG( "The client has disconnected. Terminating..." );
	this->writing = false;
	this->reading = false;
	
	LOG( "Forcing logout..." );
	bool loggedOut = Authenticator::logout( this->currentUser );
	if ( !loggedOut ) {
		LOG( "Already logged out." );
	}
	LOG( "SESSION SAFELY ENDED." );
	
}































