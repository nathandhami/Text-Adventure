#include "Watcher.hpp"
#include "NetConfig.hpp"
#include "Server.hpp"

#include <boost/thread.hpp>
#include <future>


using boost::asio::ip::tcp;


// ------------------- PUBLIC -------------------

Watcher::Watcher() {
	this->connectionAcceptor =
		std::make_shared< tcp::acceptor >( 
			this->ioService, 
			tcp::endpoint( tcp::v4(), HOST_PORT ) 
		);
//	this->running = true;
	this->startAccept();
}


Watcher::~Watcher() {}


void Watcher::run() {
	try {
		this->runnerThread.reset( new boost::thread(
			(boost::bind( &boost::asio::io_service::run, &(this->ioService) ))
		));
	} catch ( std::exception& exception ) {
		std::cerr << exception.what() << std::endl;
	}
}


void Watcher::wait() {
	this->runnerThread->join();
}


// ------------------- PRIVATE ------------------

void Watcher::startAccept() {
	Server::SessionPtr newSession = 
		std::make_shared< Session >( this->connectionAcceptor->get_io_service() );
	std::cout << "Waiting for connections..." << std::endl;
	
	this->connectionAcceptor->async_accept( 
		newSession->getSocket(),
		boost::bind( 
			&Watcher::handleAccept,
			this, newSession,
			boost::asio::placeholders::error
		)
	);
	
}




void Watcher::handleAccept( 
	std::shared_ptr< Session > newSession, 
	const boost::system::error_code& error 
) {
	if ( !error ) {
		std::async(
			std::launch::async,
			[ this, newSession ]() {
				std::cout << "Session launched." << std::endl;
				newSession->start();
			}
		);
		
		Server::registerNewSession( newSession );
		this->startAccept();
	}
}