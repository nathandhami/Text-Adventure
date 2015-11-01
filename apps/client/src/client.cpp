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

	Gtk::Main kit(argc);
	MainWindow window;

	Game::initialize();
	Game::start();

	std::cout << Game::login( "devon", "test" ).body << std::endl;
	//Game::getFrontResponse().body;

	Gtk::Main::run(window);
	
	Game::stop();

	return 0;
}