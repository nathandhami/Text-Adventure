#include <iostream>

#include "Watcher.hpp"
#include "Server.hpp"


#define MESSAGE_WELCOME		"Text Gale Online server has started."
#define MESSAGE_LISTEN		"ALl good. Awaiting the connections."
#define MESSAGE_GOODBYE		"Server has finished working... Good bye."

int main( int argc, const char* argv[] ) {
	std::cout << MESSAGE_WELCOME << std::endl;
	
	Server::initialize();
	Server::start();
	Server::waitUntilDone();
	
//	Watcher connectionWatcher;
//	connectionWatcher.run();
	
	std::cout << MESSAGE_LISTEN << std::endl;
	//TO-DO: Add controlling module to manipulate sessions and the Watcher
	
//	connectionWatcher.wait();
	
	std::cout << MESSAGE_GOODBYE << std::endl;
	
	return 0;
}