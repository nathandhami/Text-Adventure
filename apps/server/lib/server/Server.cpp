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


std::string Server::registerNewSession( Server::SessionPtr newSession ) {
	std::string identifierString;
	bool inserted = false;
	
	//Keep doing this until a unique UUID is generated (to avoid collisions)
	do {
		identifierString = Server::generateUniqueIdentifierString();
		
		//Attempt to insert the session and get the status boolean
		inserted = Server::sessions.emplace( identifierString, newSession ).second;
	} while ( !inserted );
	
	std::cout << "Registered a session with ID: " << identifierString << std::endl;
	return identifierString;
}

void Server::destroySession( std::string identifierString ) {
	if ( !Server::sessions.count( identifierString ) ) {
		std::cout << "COULD NOT DELETE THE SESSION." << std::endl;
		return;
	}
	
	Server::sessions.erase( identifierString );
}


void Server::sendMessageToClient( std::string sessionId, std::string message ) {
	std::cout << "Contatcted Server Service Locator." << std::endl;
	std::string id = Server::sessions.begin()->first;
	Server::sessions[ id ]->writeToClient( HEADER_OK, message );
}


// ------------------- PRIVATE ------------------

std::shared_ptr< Watcher > Server::watcher;
Server::SessionMap Server::sessions;


std::string Server::generateUniqueIdentifierString() {
	boost::uuids::random_generator uuidGenerator;
	boost::uuids::uuid newSessionIdentifier = uuidGenerator();
	std::string identifierString = boost::lexical_cast< std::string >( newSessionIdentifier );
	
	return identifierString;
}