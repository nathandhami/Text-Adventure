#include "Watcher.hpp"
#include "NetConfig.hpp"

#include <boost/thread.hpp>


using boost::asio::ip::tcp;


// ------------------- PUBLIC -------------------

Watcher::Watcher() {
	this->connectionAcceptor =
		std::make_shared< tcp::acceptor >( 
			this->ioService, 
			tcp::endpoint( tcp::v4(), HOST_PORT ) 
		);
	this->startAccept();
}


Watcher::~Watcher() {}


void Watcher::run() {
	try {
//		std::thread runner( this->ioService.run() );
		this->runnerThread.reset( new boost::thread(
			(boost::bind( &boost::asio::io_service::run, &(this->ioService) ))
//			tr.join();
		));
	} catch ( std::exception& exception ) {
		std::cerr << exception.what() << std::endl;
	}
}


void Watcher::wait() {
	runnerThread->join();
}


// ------------------- PRIVATE ------------------

void Watcher::startAccept() {
	std::shared_ptr< Session > newSession = 
		std::make_shared< Session >( this->ioService );
	this->sessions.push_back( newSession );
	
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
		newSession->start();
	}
	startAccept();
}