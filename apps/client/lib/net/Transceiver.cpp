#include <iostream>
#include <functional>
#include <cstdlib>
#include <chrono>
#include <iomanip>


#include "Transceiver.hpp"
#include "NetConfig.hpp"
#include "GameCode.hpp"


// Error read/write string codes
#define CODE_ERROR_READ		"rerr"
#define CODE_ERROR_WRITE	"werr"


using boost::asio::ip::tcp;


// ------------------- PUBLIC -------------------

Transceiver::Transceiver() {
	tcp::resolver resolver( ioService );
	tcp::resolver::query query( HOST_ADDRESS_IP4, std::to_string( HOST_PORT ) );
	endpointIterator = resolver.resolve( query );
	
	this->connection = std::make_shared< ServerConnection >( ioService, endpointIterator );
	
	this->connectToHost();
}


void Transceiver::run() {
	this->ioService.run();
}


void Transceiver::stop() {
	this->reading = false;
	this->ioService.stop();
	this->readerThread.detach();
}


void Transceiver::writeToServer( std::string header, std::string body ) {
	// put size of the body into a string
	std::stringstream formatStream;
	formatStream << std::setfill( '0' ) << std::setw( NetMessage::MaxLength::BODY_LENGTH ) << body.length();
	
	// write to server ensuring no disconnects
	if ( !this->write( header ) ) {
		std::cout << "Write Failed." << std::endl;
		this->pushToReadQueue( GameCode::DISCONNECTED, "Lost connection to the server." );
	} else if ( !this->write( formatStream.str() ) ) {
		std::cout << "Write Failed." << std::endl;
		this->pushToReadQueue( GameCode::DISCONNECTED, "Lost connection to the server." );
	} else if ( !this->write( body ) ) {
		std::cout << "Write Failed." << std::endl;
		this->pushToReadQueue( GameCode::DISCONNECTED, "Lost connection to the server." );
	}
}


bool Transceiver::queueEmpty() {
	return this->responseQueue.empty();
}


NetMessage Transceiver::readAndPopQueue() {
	NetMessage message = this->responseQueue.front();
	this->responseQueue.pop();
	return message;
}


void Transceiver::flushQueue() {
	this->responseQueue = std::queue<NetMessage>{};
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


bool Transceiver::write( std::string dataString ) {
	boost::system::error_code error;
	boost::asio::write(
		this->connection->getSocket(),
		boost::asio::buffer( dataString ),
		error
	);
	
	return ( error == 0 );
}


void Transceiver::asyncReadServerResponses() {
	this->readerThread = std::thread(
		[ this ]() {
			std::cout << "Reader started." << std::endl;
			this->reading = true;
			while ( this->reading ) {
//				std::string header = this->read( NetMessage::MaxLength::HEADER );
//				std::string body = this->read( NetMessage::MaxLength::BODY );
				
				
				std::string header = this->read( NetMessage::MaxLength::HEADER );
				if ( header == CODE_ERROR_READ ) {
					this->reading = false;
					this->pushToReadQueue( GameCode::DISCONNECTED, "Lost connection to the server." );
					break;
				}
				// get body length
				std::string bodyLengthStr = this->read( NetMessage::MaxLength::BODY_LENGTH );
				if ( bodyLengthStr == CODE_ERROR_READ ) {
					this->reading = false;
					this->pushToReadQueue( GameCode::DISCONNECTED, "Lost connection to the server." );
					break;
				}
				int bodyLength = atoi( bodyLengthStr.c_str() );
				// get body
				std::string body = this->read( bodyLength );
				if ( header == CODE_ERROR_READ ) {
					this->reading = false;
					this->pushToReadQueue( GameCode::DISCONNECTED, "Lost connection to the server." );
					break;
				}
				
				
				
//				if ( !this->reading ) break; // if thread sneaks to read the last time before program dies
				if ( header == CODE_ERROR_READ || body == CODE_ERROR_READ ) {
					// handle server down/loss of connection
					std::cout << "Lost connection." << std::endl;
					this->reading = false;
					this->pushToReadQueue( GameCode::DISCONNECTED, "Lost connection to the server." );
					break;
				} else {
					// write to gameresponses
					this->pushToReadQueue( header, body );
//					std::cout << "Response pushed." << std::endl;
				}
			}
			this->readerThread.detach();
		}
	);
}


std::string Transceiver::read( const int maxBufferLength ) {
//	std::cout << "Waiting for server write..." << std::endl;
	
	std::vector< char > buffer( maxBufferLength );
	boost::system::error_code error;
	
	std::size_t bufferLength = this->connection->getSocket().read_some(
		boost::asio::buffer( buffer ),
		error
	);
	
	if ( error ) {
		return CODE_ERROR_READ;
	}

	return std::string( buffer.begin(), buffer.begin() + bufferLength );
}


// Thread-safe
void Transceiver::pushToReadQueue( std::string header, std::string body ) {
	this->readQueueMutex.lock();
	// ----- critical section -----
	this->responseQueue.push( NetMessage( header, body ) );
	// --- end critical section ---
	this->readQueueMutex.unlock();
}












