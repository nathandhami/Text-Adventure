#include "UIWriter.hpp"
#include "UIReader.hpp"
#include "Transceiver.hpp"
#include "NetConfig.hpp"

#include <cstdlib>
#include <boost/algorithm/string/join.hpp>
#include <boost/algorithm/string.hpp>
#include <gtkmm.h>

#define MESSAGE_CONNECTING	"Connecting to server..."
#define MESSAGE_CONNECTED	"Connected successfuly."


//NOTE: all the static classes will be placed in a wrapper with the Transceiver later on

static std::shared_ptr< Transceiver > transceiver; 

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
		transceiver->write( HEADER_COMMAND, command );
		UIWriter::sendSysStatus( std::get< 1 >( transceiver->read() ) );
		readCommands();	
	}
}



int main( int argc, const char* argv[] ) {

	Gtk::Main kit(argc);

	Gtk::ScrolledWindow outputScrollWindow;
	Gtk::TextView outputTextview;
	Gtk::Grid uiGrid;
	Gtk::Entry commandEntry;
	Gtk::Window window;
	Gtk::Button button("Click Me");
	Gtk::Box commandBox(Gtk::ORIENTATION_HORIZONTAL, 5);
	Gtk::Frame commandFrame("Enter Command");
	
	window.set_default_size(900, 500);
	window.set_title("Text Gale Online");
	window.set_position(Gtk::WIN_POS_CENTER);
	window.set_border_width(10);

	outputScrollWindow.add(outputTextview);

	//window.add(button);
	uiGrid.set_orientation(Gtk::ORIENTATION_VERTICAL);
	uiGrid.add(commandFrame);
	uiGrid.add(commandEntry);

//	commandFrame.add(uiGrid);

	window.add(uiGrid);
	
	window.show_all_children();
	//button.show();
	//window.add(button);
	
	//kit.run(window);
	Gtk::Main::run(window);
	/*UIWriter::sendSysStatus( "Connecting to server..." );
	
	transceiver = std::make_shared< Transceiver >();
	transceiver->run();
	
	UIWriter::sendSysStatus( "Connected." );
	
	login();
	
	UIWriter::sendSysStatus( "What would you like to do?" );
	readCommands();*/

	return 0;
}