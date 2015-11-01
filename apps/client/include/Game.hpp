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
	static NetMessage login( std::string userName, std::string password );
	static void logout();
	
	
private:
	
	Game() {}
	
	// Transport module
	static std::shared_ptr< Transceiver > transceiver;
	
	
	
};


#endif