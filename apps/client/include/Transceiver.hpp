#ifndef TRANSCEIVER_HPP
#define TRANSCEIVER_HPP

//********************************************************************** 
// TRANSCEIVER
// 
// This module allows the client to connect to the server; send and 
// receive messages with headers.
//
// Author: Pavel Kozlovsky (pkozlovs@sfu.ca)
//**********************************************************************

#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <memory>
#include <tuple>

#include "ServerConnection.hpp"
#include "NetMessage.hpp"


using boost::asio::ip::tcp;


class Transceiver {

public:
	Transceiver();
	~Transceiver();
	
	void run();
	
	void write( std::string header, std::string body );
	std::tuple< std::string, std::string > read();
	
	
private:
	
	boost::asio::io_service ioService;
	tcp::resolver::iterator endpointIterator;
	std::shared_ptr< ServerConnection > connection;
	NetMessage response;

	void connectToHost();
	
	void readHeaderFromHost();
	void readBodyFromHost();
	
	void writeToHost( std::string message );
	
};

#endif
