#ifndef CONNECTION_HPP
#define CONNECTION_HPP

//********************************************************************** 
// SESSION
// 
// This module holds information about a single connection accepted
// by the server, as well as allows asynchronous interaction between 
// the client and the world.
//
// Author: Pavel Kozlovsky (pkozlovs@sfu.ca)
//**********************************************************************


#include <queue>
#include <mutex>
#include <boost/asio.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>

#include "NetMessage.hpp"


using boost::asio::ip::tcp;

class Session {
	
public:
	enum IPType { v4, v6 };
	
	Session( boost::asio::io_service& ioService ): socket( ioService ) {}
	
	tcp::socket& getSocket();
	void start();
	
private:
	
	tcp::socket socket;
	std::string clientIP_v4;
	NetMessage request;
	char bufferBody[ NetMessage::MaxLength::BODY ];
	char bufferHeader[ NetMessage::MaxLength::HEADER ];
	
	std::queue< NetMessage > responseMessageQueue;
	std::mutex messageQueueLock;
	bool writeInProgress = false;
	
	//Autherization variables
	int userId = 0;
	bool authorized = false;
	//for later
	std::size_t userHash;
	
	
	std::string getIP( IPType type );
	
	void readHeader();
	void readBody();
	
	void handleRequest();
	
	void asyncWrite();
	bool write( std::string message );
	void writeToClient( std::string header, std::string body );
};

#endif
