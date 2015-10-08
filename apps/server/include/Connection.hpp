#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#include <boost/asio.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>


using boost::asio::ip::tcp;

class Connection: public boost::enable_shared_from_this< Connection > {
	
public:
	typedef boost::shared_ptr< Connection > pointer;
	
	static pointer createPointer( boost::asio::io_service& ioService );
	tcp::socket& getSocket();
	void start();
	
private:
	tcp::socket socket;
	
	Connection( boost::asio::io_service& ioService ): socket( ioService ) {}
	void handleWrite( const boost::system::error_code& /*error*/, size_t /*bytes*/ ) {}
	
};


#endif