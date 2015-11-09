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

	argc = 1;
	Glib::RefPtr<Gtk::Application> app = Gtk::Application::create( argc, argv, "com.gtkmm.tutorial1.base" );
	
	MainWindow mainWindow;
//	GameWindow gameWindow;
	
//	gameWindow.show();
//	app->add_window( loginWindow );
//	app->run();
	app->run( mainWindow );

	return 0;
	
	
//	if(!Glib::thread_supported()) {
//		Glib::thread_init();
//	}
//	Gtk::Main kit(argc);
//	MainWindow window;
//
//	
//
//	Gtk::Main::run(window);	

//	Game::logout();
	
//	Game::stop();

//	return 0;
}
