#include "Connection.hpp"


using boost::asio::ip::tcp;


Connection::pointer Connection::createPointer( boost::asio::io_service& ioService ) {
	return pointer( new Connection( ioService ) );
}


tcp::socket& Connection::getSocket() {
	return socket;
}


void Connection::start() {
	std::string message = "PING PING";
	boost::asio::async_write(
		socket, 
		boost::asio::buffer( message ),
		boost::bind( 
			&Connection::handleWrite, 
			shared_from_this(),
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred
		)
	);
}