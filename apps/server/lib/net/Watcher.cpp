#include "Watcher.hpp"
#include "NetConfig.hpp"


using boost::asio::ip::tcp;


Watcher::Watcher() {
	this->acceptor = new tcp::acceptor( this->ioService, tcp::endpoint( tcp::v4(), boost::lexical_cast<int>( HOST_PORT ) ) );
	running = false;
}


Watcher::~Watcher() {
	delete this->acceptor;
}


void Watcher::asyncRun(  ) {
	try {
		while ( true ) {
			tcp::socket socket = tcp::socket( this->ioService );
			acceptor->accept( socket );
			std::cout << "Connection Accepted." << std::endl;
			
			//TO-DO: request information from the engine
			std::string message = "PING";
			//END-TO-DO

			boost::system::error_code ignoredError;
			boost::asio::write( socket, boost::asio::buffer( message ), ignoredError );
			std::cout << "Sent message: " << message << std::endl;
		}
	} catch ( std::exception& exception ) {
		std::cerr << exception.what() << std::endl;
	}
}


void Watcher::run() {
	this->runnerThread = std::thread( &Watcher::asyncRun, this );
	this->runnerThread.join();
}


bool Watcher::getRunningState() {
	bool stateHolder;
	this->stateMutex.lock();

	stateHolder = this->running;

	this->stateMutex.unlock();

	return stateHolder;
}


void Watcher::setRunningState( bool state ) {
	this->stateMutex.lock();
	
	this->running = state;
	
	this->stateMutex.unlock();
}