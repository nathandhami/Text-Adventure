#ifndef CONNECTION_HPP
#define CONNECTION_HPP

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
	
	//Autherization variables
	int userId = 0;
	bool authorized = false;
	//for later
	std::size_t userHash;
	
	std::string getIP( IPType type );
	
	void readHeader();
	void readBody();
	
	void handleRequest();
	
	bool write( std::string message );
	void writeToClient( std::string header, std::string body );
};

#endif
