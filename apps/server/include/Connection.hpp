#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#include <boost/asio.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>


using boost::asio::ip::tcp;

class Connection {
	
public:
	Connection( boost::asio::io_service& ioService ): socket( ioService ) {}
	
	tcp::socket& getSocket();
	void start();
	
private:
	tcp::socket socket;
	
	void handleWrite( const boost::system::error_code& /*error*/, size_t /*bytes*/ ) {}
	
};


#endif