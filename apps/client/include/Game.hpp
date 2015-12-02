#ifndef GAME_HPP
#define GAME_HPP


#include <memory>
#include <mutex>


#include "GameCode.hpp"
#include "Transceiver.hpp"
#include "NetMessage.hpp"


class Game {
public:
	
	static void initialize();
	static void start();
	static void stop();
	static NetMessage getFrontResponse();
	
	static NetMessage registerUser( std::string userName, std::string password, std::string passwordRep );
	static NetMessage login( std::string userName, std::string password );
	static void logout();
	static NetMessage createCharacter( std::string charName, std::string charDesc );
	static NetMessage deleteCharacter( std::string charName );
	static NetMessage selectCharacter( std::string charName );
	static NetMessage deselectCurrentCharacter();
	static void enact( std::string userInputString );
	
	
private:
	
	Game() {}
	
	// Transport module
	static std::shared_ptr< Transceiver > transceiver;
	
	static NetMessage getBusyResponse();
	
	
	
};


#endif