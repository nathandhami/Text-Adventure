#ifndef WATCHER_HPP
#define WATCHER_HPP

//********************************************************************** 
// WATCHER
// 
// This module serves as a connection listener for this application. 
// Upon accepting a connection, it is passed as a session to a new
// thread and handled there onwards.
//
// Author: Pavel Kozlovsky (pkozlovs@sfu.ca)
//**********************************************************************

#include <boost/asio.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/thread.hpp>
#include <thread>
#include <memory>

#include "Session.hpp"

using boost::asio::ip::tcp;

class Watcher {
public:
	Watcher();
	~Watcher();

	void run();
	void wait();
	
private:
	boost::asio::io_service ioService;
	std::shared_ptr< tcp::acceptor > connectionAcceptor;
	boost::scoped_ptr<boost::thread> runnerThread;
	
//	bool running;
	
	void startAccept();
	void handleAccept( 
		std::shared_ptr< Session > newSession, 
		const boost::system::error_code& error 
	);
};


#endif