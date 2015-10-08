#ifndef WATCHER_HPP
#define WATCHER_HPP

#include <boost/asio.hpp>
#include <boost/lexical_cast.hpp>
#include <thread>
#include <mutex>

#include "Connection.hpp"

using boost::asio::ip::tcp;

class Watcher {
public:
	Watcher();
	~Watcher();

	void run();
	
private:
	boost::asio::io_service ioService;
	tcp::acceptor* connectionAcceptor;
	
	void startAccept();
	void handleAccept( 
		Connection::pointer newConnection, 
		const boost::system::error_code& error 
	);
};


#endif