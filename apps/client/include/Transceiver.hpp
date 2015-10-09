#ifndef TRANSCEIVER_HPP
#define TRANSCEIVER_HPP

#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <memory>

#include "ServerConnection.hpp"
#include "NetMessage.hpp"


using boost::asio::ip::tcp;


class Transceiver {

public:
	Transceiver();
	~Transceiver();
	
	void run();
	
	void write( std::string message );
	std::string read();
	
	
private:
	
	boost::asio::io_service ioService;
	tcp::resolver::iterator endpointIterator;
	std::shared_ptr< ServerConnection > connection;
	NetMessage response;

	void connectToHost();
	
	void readFromHost();
	void writeToHost( std::string message );
	
};


#endif