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
	std::cout << "- Tried to log out." << std::endl;
}


// BLOCKING
// TO-DO: make blocking
/*void Game::selectCharacter( std::string charNameOrNumber ) {
	Game::transceiver->writeToServer( GameCode::CHAR_SELECT, charNameOrNumber );
	std::cout << "- Tried to select a char." << std::endl;
}*/


void Game::enact( std::string userInputString ) {
	Game::transceiver->writeToServer( GameCode::COMMAND, userInputString );
	std::cout << "- Tried to command." << std::endl;
}


// ------------------- PRIVATE ------------------

std::shared_ptr< Transceiver > Game::transceiver;