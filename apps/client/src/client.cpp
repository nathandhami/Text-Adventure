#include "UIWriter.hpp"
#include "UIReader.hpp"
#include "Transceiver.hpp"
#include "NetConfig.hpp"
#include "MainWindow.hpp"
#include "Game.hpp"

#include <cstdlib>
#include <boost/algorithm/string/join.hpp>
#include <boost/algorithm/string.hpp>
#include <gtkmm.h>

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

static void on_enter_pressed()
{
	std::cout << "Enter pressed." << std::endl;
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

int main( int argc, const char* argv[] ) {

	if(!Glib::thread_supported()) {
		Glib::thread_init();
	}
	Gtk::Main kit(argc);
	MainWindow window;

	//Game::initialize();
	//Game::start();

	//NetMessage response = Game::login( "testUser1", "test1" );
	//NetMessage response = Game::login( argv[ 1 ], argv[ 2 ] );
	
	//std::cout << "Got response:" << std::endl;
	//std::cout << "\tHeader: " << response.header << std::endl;
	//std::cout << "\tBody:\n" << response.body << std::endl;
	
//	Game::enact( "go north" );
//	Game::enact( "move north" );
//	Game::enact( "look up" );
//	Game::enact( "move east" );
//	Game::enact( " look east" );
//	Game::enact( "fight sadas" );
//	Game::enact( "@ testChar1 do you think this is a game?" );
//	Game::enact( "# This is a zone MeSsAgE." );
//	Game::enact( "blergh" );
//	Game::enact( "create zone my head hurts" );
//	Game::enact( "create zone [Deadman Wonderland] seen as [You see a what used to be colourful arc with giant letters spelling: 'Deadman Wonderland']" );
//	Game::enact( "describe zone [3054] as [prisoners] telling [It's also known as hell. Prisoners go here to enjoy their last days in deadly 		competetive games for food and water.]" );
//	Game::enact( "delete zone 3207" );
//	Game::enact( "create door [river] in [3054] at [east] to [3205] seen as [You see river glittering in the distance playing the melody of 	spring.]" );
//	Game::enact( "delete door 235" );
	
//	std::cout << "Response: " << response.body << std::endl;
	
	/*response = Game::login( "testUser2", "test2" );
	
	std::cout << "Got response:" << std::endl;
	std::cout << "\tHeader: " << response.header << std::endl;
	std::cout << "\tBody:\n" << response.body << std::endl;
	
	Game::logout();
	
	std::cout << "Response: " << response.body << std::endl;*/
//	int i = 10;
	
	/*while( true ) {
		NetMessage response = Game::getFrontResponse();

		if ( response.header != GameCode::NONE ) {
			std::cout << "Response: " << response.body << std::endl;
		}
		
		
		std::this_thread::sleep_for( std::chrono::milliseconds( 10 ) );
	}*/

	Gtk::Main::run(window);	

	Game::logout();
	
	Game::stop();

	return 0;
}
