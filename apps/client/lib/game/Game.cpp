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
//	std::vector< std::string > credentialTokens = { userName, password, passwordRep };
//	std::string joinedCredentials = boost::algorithm::join( credentialTokens, ";" );
	std::string joinedCredentials = userName + ";" + password + ";" + passwordRep;
	
	Game::transceiver->writeToServer( GameCode::REGISTER, joinedCredentials );
	return Game::getBusyResponse();
}


// BLOCKING
NetMessage Game::login( std::string userName, std::string password ) {
//	std::vector< std::string > credentialTokens = { userName, password };
//	std::string joinedCredentials = boost::algorithm::join( credentialTokens, ";" );
	std::string joinedCredentials = userName + ";" + password;
	
	Game::transceiver->writeToServer( GameCode::LOGIN, joinedCredentials );
	
	while ( Game::transceiver->queueEmpty() ) {}
	return Game::transceiver->readAndPopQueue();
}


void Game::logout() {
	Game::transceiver->writeToServer( GameCode::LOGOUT, "arbitrary string" );
	std::cout << "[Game] Tried to log out." << std::endl;
	
	/*while ( Game::getFrontResponse().header != GameCode::OK ) {
//		std::cout << "[Game] Draining for log out" << std::endl;
	}
	*/
	
	while ( true ) {
		NetMessage nmIter = Game::getFrontResponse();
		if ( nmIter.header == GameCode::LOGOUT ) break;
	}
	
	Game::transceiver->flushQueue();
	std::cout << "[Game] Logged out successfully." << std::endl;
}


// BLOCKING
NetMessage Game::createCharacter( std::string charName, std::string charDesc ) {
	std::string charData = charName + "|" + charDesc;
	Game::transceiver->writeToServer( GameCode::CHAR_CREATE, charData );
	return Game::getBusyResponse();
}


// BLOCKING
NetMessage Game::deleteCharacter( std::string charName ) {
	Game::transceiver->writeToServer( GameCode::CHAR_DELETE, charName );
	return Game::getBusyResponse();
}


// BLOCKING
NetMessage Game::selectCharacter( std::string charName ) {
	Game::transceiver->writeToServer( GameCode::CHAR_SELECT, charName );
	std::cout << "[Game] Tried to select a char." << std::endl;
	return Game::getBusyResponse();
}


void Game::deselectCurrentCharacter() {
	Game::transceiver->writeToServer( GameCode::CHAR_DELECT, "_" );
	std::cout << "[Game] Tried to delect a char." << std::endl;
	NetMessage nmIter;
	while ( true ) {
		nmIter = Game::getFrontResponse();
		if ( nmIter.header == GameCode::CHAR_DELECT ) break;
	}

	Game::transceiver->flushQueue();
}


void Game::enact( std::string userInputString ) {
	Game::transceiver->writeToServer( GameCode::COMMAND, userInputString );
	std::cout << "- Tried to command." << std::endl;
}


// ------------------- PRIVATE ------------------

std::shared_ptr< Transceiver > Game::transceiver;


NetMessage Game::getBusyResponse() {
	while ( Game::transceiver->queueEmpty() ) {}
	return Game::transceiver->readAndPopQueue();
}