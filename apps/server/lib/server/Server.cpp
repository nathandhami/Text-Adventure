#include "Server.hpp"

#include <iostream>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/lexical_cast.hpp>


// ------------------- PUBLIC -------------------

void Server::initialize() {
	Server::watcher = std::make_shared< Watcher >();
}


void Server::start() {
	Server::watcher->run();
}


void Server::waitUntilDone() {
	Server::watcher->wait();
}


void Server::registerNewSession( Server::SessionPtr newSession ) {
	std::string identifierString;
	bool inserted = false;
	
	//Keep doing this until a unique UUID is generated (to avoid collisions)
	do {
		boost::uuids::random_generator uuidGenerator;
		boost::uuids::uuid newSessionIdentifier = uuidGenerator();
		identifierString = boost::lexical_cast< std::string >( newSessionIdentifier );
		
		//Attempt to insert the session and get the status boolean
		inserted = Server::sessions.emplace( identifierString, newSession ).second;
	} while ( !inserted );
	
	std::cout << "Registered a session with ID: " << identifierString << std::endl;
}

// ------------------- PRIVATE ------------------

std::shared_ptr< Watcher > Server::watcher;
Server::SessionMap Server::sessions;