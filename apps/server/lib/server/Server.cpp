#include "Server.hpp"
#include "DatabaseTool.hpp"

#include <iostream>
#include <cstdlib>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/lexical_cast.hpp>


// ------------------- PUBLIC -------------------

void Server::initialize() {
	Server::watcher = std::make_shared< Watcher >();
}


void Server::start() {
	if ( !DatabaseTool::testValidity() ) exit( EXIT_FAILURE );
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


bool Server::sendMessageToCharacter( int characterId, std::string header, std::string body ) {
	std::cout << "Contacted Server Service Locator." << std::endl;
	
	std::string sessionIdString = DatabaseTool::getSessionID( characterId );
	std::cout << "[Server] got ID: " << sessionIdString << std::endl;
	if ( sessionIdString == "" ) {
		return false;
	}
	
	if ( !Server::sessions.count( sessionIdString ) ) {
		std::cout << "[Server] No process associated with this session." << std::endl;
		return false;
	}
	
	std::cout << "[Server] Sending message to " << characterId << ": ok." << std::endl;
	Server::sessions[ sessionIdString ]->writeToClient( header, body );
	
	return true;
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