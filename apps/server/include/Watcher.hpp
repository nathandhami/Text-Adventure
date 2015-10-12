#ifndef WATCHER_HPP
#define WATCHER_HPP

#include <boost/asio.hpp>
#include <boost/lexical_cast.hpp>
#include <thread>
#include <memory>

#include "Session.hpp"

using boost::asio::ip::tcp;

class Watcher {
public:
	Watcher();
	~Watcher();

	void run();
	
private:
	boost::asio::io_service ioService;
	std::shared_ptr< tcp::acceptor > connectionAcceptor;
	std::vector< std::shared_ptr< Session > > sessions;
	
	void startAccept();
	void handleAccept( 
		std::shared_ptr< Session > newSession, 
		const boost::system::error_code& error 
	);
};

#endif
