#include "UIWriter.hpp"
#include "UIReader.hpp"
#include "Transceiver.hpp"
#include "NetConfig.hpp"
#include "Game.hpp"

#include <cstdlib>
#include <boost/algorithm/string/join.hpp>
#include <boost/algorithm/string.hpp>


#define MESSAGE_CONNECTING	"Connecting to server..."
#define MESSAGE_CONNECTED	"Connected successfuly."


//NOTE: all the static classes will be placed in a wrapper with the Transceiver later on

/*static std::shared_ptr< Transceiver > transceiver; 

static std::string joinCredentials( std::string userName, std::string password ) {
	std::vector<std::string> tokens;
    tokens.push_back( userName );
    tokens.push_back( password );

    return boost::algorithm::join( tokens, ";" );
}


static void login() {
	UIWriter::sendSysStatus( "Please enter your credentials" );
	
	std::string userName = UIReader::getUserName();
	std::string password = UIReader::getPassword();
	std::string credentialString = joinCredentials( userName, password );
	transceiver->write( HEADER_LOGIN, credentialString );
	if ( std::get< 0 >( transceiver->read() ) == HEADER_ERROR ) {
		UIWriter::sendSysStatus( std::get< 1 >( transceiver->read() ) );
		login();
	} else {
		UIWriter::sendSysStatus( std::get< 1 >( transceiver->read() ) );
	}
}

static void readCommands() {
	std::string command = UIReader::getCommand();
	boost::to_lower( command );
	if ( command == "logout" ) {
		transceiver->write( HEADER_LOGOUT, command );
		UIWriter::sendSysStatus( std::get< 1 >( transceiver->read() ) );
	} else {
//		transceiver->write( HEADER_COMMAND, command );
		transceiver->write( HEADER_MESSAGE, command );
		UIWriter::sendSysStatus( std::get< 1 >( transceiver->read() ) );
		readCommands();	
	}
}*/



int main( /*int argc, const char* argv[]*/ ) {
//	UIWriter::sendSysStatus( "Connecting to server..." );
//	
//	transceiver = std::make_shared< Transceiver >();
//	transceiver->run();
//	
//	UIWriter::sendSysStatus( "Connected." );
//	
//	login();
//	
//	UIWriter::sendSysStatus( "What would you like to do?" );
//	readCommands();
	
	
	
	Game::initialize();
	Game::start();
	
	NetMessage response = Game::login( "testUser1", "test1" );
	
	std::cout << "Got response:" << std::endl;
	std::cout << "\tHeader: " << response.header << std::endl;
	std::cout << "\tBody:\n" << response.body << std::endl;
	
//	Game::enact( "look" );
//	Game::enact( "move north" );
	
	Game::logout();
	
	std::cout << "Response: " << response.body << std::endl;
	
	response = Game::login( "testUser2", "test2" );
	
	std::cout << "Got response:" << std::endl;
	std::cout << "\tHeader: " << response.header << std::endl;
	std::cout << "\tBody:\n" << response.body << std::endl;
	
	Game::logout();
	
	std::cout << "Response: " << response.body << std::endl;
	
	while( 1 ) {
		
		std::cout << "Response: " << Game::getFrontResponse().body << std::endl;
		std::this_thread::sleep_for( std::chrono::milliseconds( 1000 ) );
	}
	Game::getFrontResponse();
	
	Game::stop();
	
	
	return 0;
}