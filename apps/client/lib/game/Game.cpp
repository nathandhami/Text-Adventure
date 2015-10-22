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
	while ( Game::transceiver->queueEmpty() ) {
		std::cout << "Happened at least once." << std::endl;
	}
//	NetMessage msg = Game::transceiver->readAndPopQueue();

	return Game::transceiver->readAndPopQueue();
}


void Game::login( std::string userName, std::string password ) {
	std::vector< std::string > credentialTokens { userName, password };
	std::string joinedCredentials = boost::algorithm::join( credentialTokens, ";" );
	
	Game::transceiver->writeToServer( GameCode::LOGIN, joinedCredentials );
//	std::tuple< bool, NetMessage > readResult = Game::transceiver->immediateReadServerResponse();
//	
//	if ( std::get< 0 >( readResult ) ) {
//		if ( std::get< 1 >( readResult ).header == HEADER_ERROR ) {
//			response.type = GameCode::WRONG;
//		} else {
//			response.type = GameCode::CORRECT;
//		}
//		response.data = std::get< 1 >( readResult ).body;
//	} else {
//		response.type = GameCode::DC;
//	}
}


// ------------------- PRIVATE ------------------

std::shared_ptr< Transceiver > Game::transceiver;