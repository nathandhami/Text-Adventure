#include "Watcher.hpp"
#include "NetConfig.hpp"


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
		this->ioService.run();
	} catch ( std::exception& exception ) {
		std::cerr << exception.what() << std::endl;
	}
}


// ------------------- PRIVATE ------------------

void Watcher::startAccept() {
	std::shared_ptr< Session > newSession = 
		std::make_shared< Session >( this->connectionAcceptor->get_io_service() );
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
