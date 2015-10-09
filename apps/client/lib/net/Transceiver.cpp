#include <iostream>
#include <functional>

#include "Transceiver.hpp"
#include "NetConfig.hpp"


using boost::asio::ip::tcp;


Transceiver::Transceiver() {
	tcp::resolver resolver( ioService );
	tcp::resolver::query query( HOST_ADDRESS_IP4, std::to_string( HOST_PORT ) );
	endpointIterator = resolver.resolve( query );
	
	this->connection = std::make_shared< ServerConnection >( ioService, endpointIterator );
	this->connectToHost();
}



Transceiver::~Transceiver() {}


void Transceiver::connectToHost() {
	boost::asio::async_connect(
		this->connection->getSocket(),
		this->endpointIterator,
		std::bind( &Transceiver::handleConnect, this )
	);
}


void Transceiver::handleConnect() {
	std::cout << "Connection Established" << std::endl;
	this->readFromHost();
}

void test(  boost::array<char, 128> buf, std::size_t bytes ) {
	std::cout.write( buf.data(), bytes );
}

void Transceiver::readFromHost() {
	//temp to test, will be replaced
	boost::asio::async_read_until(
		this->connection->getSocket(), 
		this->response,
		'\n',
		[this](boost::system::error_code ec, std::size_t /*length*/){
			std::istream response_stream(&response);
			std::string status_message;
			std::getline(response_stream, status_message);
			std::cout << status_message << std::endl;
		}
	);
}


void Transceiver::run() {
	this->ioService.run();
}