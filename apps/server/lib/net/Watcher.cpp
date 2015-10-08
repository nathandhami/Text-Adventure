#include "Watcher.hpp"
#include "NetConfig.hpp"


using boost::asio::ip::tcp;


Watcher::Watcher() {
	this->connectionAcceptor = 
		new tcp::acceptor( this->ioService, tcp::endpoint( tcp::v4(), HOST_PORT ) );
	this->startAccept();
}


Watcher::~Watcher() {
	delete this->connectionAcceptor;
}

void Watcher::run() {
	try {
		this->ioService.run();
	} catch ( std::exception& exception ) {
		std::cerr << exception.what() << std::endl;
	}
}

void Watcher::startAccept() {
	Connection::pointer newConnection = 
		Connection::createPointer( this->connectionAcceptor->get_io_service() );
	
	this->connectionAcceptor->async_accept( 
		newConnection->getSocket(),
		boost::bind( &Watcher::handleAccept,
			this, newConnection,
			boost::asio::placeholders::error
		)
	);
	
}

void Watcher::handleAccept( 
	Connection::pointer newConnection, 
	const boost::system::error_code& error 
) {
	if ( !error ) {
		newConnection->start();
	}
	startAccept();
}