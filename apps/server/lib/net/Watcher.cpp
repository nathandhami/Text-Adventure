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


void Watcher::run() {
	std::thread( asyncRun );
}

void Watcher::asyncRun() {
	
}

void Watcher::setRunningState( bool state ) {
	this->stateMutex.lock();
	
	this->running = state;
	
	this->stateMutex.unlock();
}

bool Watcher::getRunningState() {
	bool stateHolder;
	this->stateMutex.lock();
	
	stateHolder = this->running;
	
	this->stateMutex.unlock();
	
	return stateHolder;
}