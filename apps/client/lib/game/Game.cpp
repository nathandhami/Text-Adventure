#include "Game.hpp"
#include "GameCode.hpp"
#include "NetMessage.hpp"


#include <boost/algorithm/string/join.hpp>
#include <boost/algorithm/string.hpp>



// ------------------- PUBLIC -------------------

void Game::initialize() {
	Game::transceiver = std::make_shared< Transceiver >();
}


void Game::start() {
	Game::transceiver->run();
	Game::transceiver->asyncReadServerResponses();
}


void Game::stop() {
	Game::transceiver->stop();
}


NetMessage Game::getFrontResponse() {
	NetMessage responseMessage( GameCode::NONE, "" );
	if ( !Game::transceiver->queueEmpty() ) {
		responseMessage = Game::transceiver->readAndPopQueue();
	}

	return responseMessage;
}

// BLOCKING
NetMessage Game::registerUser( std::string userName, std::string password, std::string passwordRep ) {
	std::vector< std::string > credentialTokens = { userName, password, passwordRep };
	std::string joinedCredentials = boost::algorithm::join( credentialTokens, ";" );
	
	Game::transceiver->writeToServer( GameCode::REGISTER, joinedCredentials );
	
	while ( Game::transceiver->queueEmpty() ) {}
	return Game::transceiver->readAndPopQueue();
}


// BLOCKING
NetMessage Game::login( std::string userName, std::string password ) {
	std::vector< std::string > credentialTokens = { userName, password };
	std::string joinedCredentials = boost::algorithm::join( credentialTokens, ";" );
	
	Game::transceiver->writeToServer( GameCode::LOGIN, joinedCredentials );
	
	while ( Game::transceiver->queueEmpty() ) {}
	return Game::transceiver->readAndPopQueue();
}


// TO-DO: wait for logout or DC response, implement queue flushing, reset reading and writing
void Game::logout() {
	Game::transceiver->writeToServer( GameCode::LOGOUT, "arbitrary string" );
	std::cout << "[Game] Tried to log out." << std::endl;
	
	NetMessage nmIter;
	while ( Game::getFrontResponse().header != GameCode::OK ) {
//		std::cout << "[Game] Draining for log out" << std::endl;
	}
	
	Game::transceiver->flushQueue();
	std::cout << "[Game] Logged out successfully." << std::endl;
}


// BLOCKING
NetMessage Game::selectCharacter( std::string charName ) {
	Game::transceiver->writeToServer( GameCode::CHAR_SELECT, charName );
	std::cout << "[Game] Tried to select a char." << std::endl;
	
	while ( Game::transceiver->queueEmpty() ) {}
	return Game::transceiver->readAndPopQueue();
}


void Game::enact( std::string userInputString ) {
	Game::transceiver->writeToServer( GameCode::COMMAND, userInputString );
	std::cout << "- Tried to command." << std::endl;
}


// ------------------- PRIVATE ------------------

std::shared_ptr< Transceiver > Game::transceiver;