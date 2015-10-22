#ifndef GAME_HPP
#define GAME_HPP


#include <memory>
#include <mutex>


#include "GameCode.hpp"
#include "Transceiver.hpp"


class Game {
public:
	
	struct Response {
		GameCode type;
		std::string data;
	};
	
	static void initialize();
	static void start();
	static void stop();
	static Response getFrontResponse();
	static void login( std::string userName, std::string password );
	static void logout();
	
	
private:
	
	Game() {}
	
	// Transport module
	static std::shared_ptr< Transceiver > transceiver;
	
	
	
};


#endif