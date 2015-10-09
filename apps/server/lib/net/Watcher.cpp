#include "Watcher.hpp"
#include "NetConfig.hpp"


using boost::asio::ip::tcp;


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

void Watcher::startAccept() {
	std::shared_ptr< Connection > newConnection = 
		std::make_shared< Connection >( this->connectionAcceptor->get_io_service() );
	this->connections.push_back( newConnection );
	
	this->connectionAcceptor->async_accept( 
		newConnection->getSocket(),
		boost::bind( &Watcher::handleAccept,
			this, newConnection,
			boost::asio::placeholders::error
		)
	);
	
}

void Watcher::handleAccept( 
	std::shared_ptr< Connection > newConnection, 
	const boost::system::error_code& error 
) {
	if ( !error ) {
		newConnection->start();
	}
	startAccept();
}