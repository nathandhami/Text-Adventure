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

#include <boost/asio.hpp>
#include <memory>
#include <tuple>
#include <queue>
#include <thread>
#include <mutex>

#include "ServerConnection.hpp"
#include "NetMessage.hpp"


using boost::asio::ip::tcp;


class Transceiver {

public:
	Transceiver();
	~Transceiver() {}
	
	void run();
	void stop();
	
//	void write( std::string header, std::string body );
//	std::tuple< std::string, std::string > read();
	
	bool writeToServer( std::string header, std::string body );
	
	bool queueEmpty();
	NetMessage readAndPopQueue();
	
	void asyncReadServerResponses();
	
private:
	
	// Server connection members
	boost::asio::io_service ioService;
	tcp::resolver::iterator endpointIterator;
	std::shared_ptr< ServerConnection > connection;
	
	// Read/write threads
	std::thread readerThread;
	bool reading;
	
	// Response queue
	std::queue< NetMessage > responseQueue;
	std::mutex responseQueueLock;
	
	
	NetMessage response;
	
	bool write( std::string dataString );
	std::string read( const int maxBufferLength );

	void connectToHost();
//	
//	void readHeaderFromHost();
//	void readBodyFromHost();
//	
//	void writeToHost( std::string message );
	
};

#endif
