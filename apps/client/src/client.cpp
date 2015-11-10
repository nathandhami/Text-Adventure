#include "UIWriter.hpp"
#include "UIReader.hpp"
#include "Transceiver.hpp"
#include "NetConfig.hpp"
//#include "MainWindow.hpp"
#include "Game.hpp"
#include <ui/MainWindow.hpp>

#include <cstdlib>
#include <boost/algorithm/string/join.hpp>
#include <boost/algorithm/string.hpp>
#include <gtkmm.h>

#define MESSAGE_CONNECTING	"Connecting to server..."
#define MESSAGE_CONNECTED	"Connected successfuly."


int main( int argc, char* argv[] ) {
	Game::initialize();
	Game::start();
	
	argc = 1;
	Glib::RefPtr< Gtk::Application > app = Gtk::Application::create( argc, argv, "ca.sfu.cmpt373.textgaleonline" );
	app->set_flags( Gio::ApplicationFlags::APPLICATION_NON_UNIQUE );
	
	MainWindow mainWindow;
	app->run( mainWindow );
	
	Game::stop();

	return 0;
}
