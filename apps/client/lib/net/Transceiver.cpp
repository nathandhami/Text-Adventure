#include <iostream>
#include <functional>
#include <cstdlib>

#include "Transceiver.hpp"
#include "NetConfig.hpp"


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

/*void Transceiver::write( std::string header, std::string body ) {
	this->writeToHost( header );
	this->writeToHost( body );
	this->readHeaderFromHost();
	this->readBodyFromHost();
}*/

bool Transceiver::writeToServer( std::string header, std::string body ) {
	return ( this->write( header ) && this->write( body ) );
}


bool Transceiver::queueEmpty() {
	return this->responseQueue.empty();
}


NetMessage Transceiver::readAndPopQueue() {
	NetMessage message = this->responseQueue.front();
	this->responseQueue.pop();
	return message;
}
/*std::tuple< bool, NetMessage > Transceiver::immediateReadServerResponse() {
	std::string header = this->read( NetMessage::MaxLength::HEADER );
	std::string body = this->read( NetMessage::MaxLength::BODY );
	
	bool readSuccess = false;
	NetMessage responseMessage;
	
	if ( header != CODE_ERROR_READ && body != CODE_ERROR_READ ) {
		readSuccess = true;
		responseMessage.header = header;
		responseMessage.body = body;
	}

	return std::make_tuple( readSuccess, responseMessage );
}*/


/*std::tuple< std::string, std::string > Transceiver::read() {
	return std::make_tuple( 
		this->response.getHeader(), 
		this->response.getBody() 
	);
}*/

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
//				std::cout << "Waiting for server write..." << std::endl;
				std::string header = this->read( NetMessage::MaxLength::HEADER );
				std::string body = this->read( NetMessage::MaxLength::BODY );
				if ( !this->reading ) break; // if thread sneaks to read the last time before program dies
				if ( header == CODE_ERROR_READ || body == CODE_ERROR_READ ) {
					// handle server down/loss of connection
					std::cout << "Lost connection." << std::endl;
					this->reading = false;
					//this->responseQueue.push( NetMessage( GameCode::DISCONNECT,  ) );
				} else {
					// write to gameresponses
					NetMessage response( header, body );
					this->responseQueue.push( response );
					std::cout << "Response pushed." << std::endl;
				}
			}
			this->readerThread.detach();
		}
	);
}


std::string Transceiver::read( const int maxBufferLength ) {
	std::cout << "Waiting for server write..." << std::endl;
	
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


/*void Transceiver::readHeaderFromHost() {
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
}*/


/*void Transceiver::readBodyFromHost() {
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
}*/


/*void Transceiver::writeToHost( std::string message ) {
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
}*/
