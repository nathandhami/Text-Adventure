#include "Connection.hpp"


using boost::asio::ip::tcp;


tcp::socket& Connection::getSocket() {
	return this->socket;
}


void Connection::start() {
	std::cout << "Client Connected" << std::endl;
	std::string message = "Size: 4\n";
	boost::asio::async_write(
		this->socket, 
		boost::asio::buffer( message ),
		boost::bind( 
			&Connection::handleWrite, 
			this,
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred
		)
	);
}