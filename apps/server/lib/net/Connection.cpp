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
	
//	boost::system::error_code error;
//	while ( true ) {
//		boost::asio::read_until(
//			this->socket,
//			this->request,
//			'\n',
//			error
//		);
//		if ( error ) { throw boost::system::system_error( error ); }
//		std::cout << "PING" << std::endl;
//
//		std::istream responseStream( &(this->request) );
//		std::string message;
//		std::getline( responseStream, message );
//		std::cout << message << std::endl;
//		
//		boost::asio::write(
//			this->socket,
//			boost::asio::buffer( message ),
//			error
//		);
//		if ( error ) { throw boost::system::system_error( error ); }
//	}
//	boost::system::error_code error;
//	boost::asio::write(
//		this->socket,
//		boost::asio::buffer( message )
//		
//	);
//	if ( error ) { throw boost::system::system_error( error ); }
//	
//	this->read();
//	boost::asio::async_write(
//		this->socket, 
//		boost::asio::buffer( message ),
//		[ this ]( boost::system::error_code error, std::size_t /*length*/ ){
//			if ( !error ) {
//				this->read();
//			}
//		}
//	);
}


// ------------------- PRIVATE ------------------

void Connection::start() {
	
	std::string address( this->getIP( Connection::IPType::v4 ) );
	std::cout << "Client Connected with ipv4: " << address << std::endl;
	std::string message = "Size: 4\n";
	this->write( message );
}


std::string Connection::getIP( IPType type ) {
	return this->socket.remote_endpoint().address().to_string();
}

void Connection::read() {
	char buffer[512];
	
	boost::system::error_code error;
	size_t length = this->socket.read_some(
		boost::asio::buffer( buffer ),
		error
	);
	if ( !error ) { 
		this->request.copyToBuffer( buffer, length );
		this->write( this->request.getBody() );
	}
//	if ( error ) { throw boost::system::system_error( error ); }
//	std::cout << "PING" << std::endl;
//	
//	std::istream responseStream( &(this->request) );
//	std::string message;
//	std::getline( responseStream, message );
//	std::cout << message << std::endl;
//	
//	this->write( "Ping" );
	
//	boost::asio::async_read_until(
//		this->socket,
//		this->request,
//		'\n',
//		[ this ]( boost::system::error_code error, std::size_t /*length*/ ){
//			if ( !error ){
//				std::cout << "PING" << std::endl;
//				this->write( "Ping" );
//			}
//		}
//	);
}