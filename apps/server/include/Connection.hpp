#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#include <boost/asio.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>


using boost::asio::ip::tcp;

class Connection {
	
public:
	enum IPType { v4, v6 };
	
	Connection( boost::asio::io_service& ioService ): socket( ioService ) {}
	
	tcp::socket& getSocket();
	void start();
	
	void write( std::string message );
	
private:
	tcp::socket socket;
	std::string clientIP_v4;
	boost::asio::streambuf request;
	
//	void handleWrite( const boost::system::error_code& /*error*/, size_t /*bytes*/ ) {}
	std::string getIP( IPType type );
	
	void read();
};


#endif