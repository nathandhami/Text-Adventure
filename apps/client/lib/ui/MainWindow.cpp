#include <ui/MainWindow.hpp>
#include <ui/SubWindow.hpp>
#include <Locale.hpp>
#include "Game.hpp"

#define WIDTH_DEFAULT 	900
#define HEIGHT_DEFAULT	500

#define BORDER_WIDTH_DEFAULT 1


// ------------------- PUBLIC -------------------

MainWindow::MainWindow() {
	this->set_title( GameLocale::TITLE );

	this->set_position( Gtk::WIN_POS_CENTER );
	this->set_default_size( WIDTH_DEFAULT, HEIGHT_DEFAULT );
	this->set_size_request( WIDTH_DEFAULT, HEIGHT_DEFAULT );
	this->set_resizable( false );
	
//	this->set_border_width( BORDER_WIDTH_DEFAULT );

	this->setupStyleSheet();
//	this->setupComponents();
	this->openLoginFrame();
//	this->openCreateFrame();
	
//	this->show_all_children();
}


void MainWindow::openLoginFrame() {
	this->remove();
	this->add( this->loginFrame );
	this->show_all_children();
	this->set_name( "window-login" );
}


void MainWindow::openRegisterFrame() {
	this->remove();
	this->add( this->registerFrame );
	this->registerFrame.show();
}



void MainWindow::openCharacterFrame() {
	this->remove();
	this->add( this->characterFrame );
	this->characterFrame.show();
	this->set_name( "window-character" );
}


void MainWindow::openCharacterFrame( std::string charList ) {
	this->remove();
	this->add( this->characterFrame );
	this->characterFrame.updateCharacterList( charList );
	this->characterFrame.show();
	this->set_name( "window-character" );
}


void MainWindow::openCreateFrame() {
	this->remove();
	this->add( this->createFrame );
	this->createFrame.show();
	this->set_name( "window-character" );
}


void MainWindow::openGameFrame() {
	this->remove();
	this->add( this->gameFrame );
	this->gameFrame.startReading();
	this->gameFrame.show();
}


// ------------------- PRIVATE ------------------

void MainWindow::setupStyleSheet() {
	const std::string PATH_CSS = "res/css/main.css";
	auto cssProvider = Gtk::CssProvider::create();
	cssProvider->load_from_path( PATH_CSS );

	auto screen = Gdk::Screen::get_default();
	auto cssContext = this->get_style_context();
	cssContext->add_provider_for_screen( screen, cssProvider, GTK_STYLE_PROVIDER_PRIORITY_APPLICATION );
}


void MainWindow::setupComponents() {
	
//	this->loginFrame.signal_hide().connect( sigc::mem_fun( *this, &MainWindow::switchToGameFrame ) );
	
	
	this->add( this->loginFrame );
	this->loginFrame.show();
	
}

void MainWindow::closeSubWindow() {
	
}


void MainWindow::switchToGameFrame() {
	/*Gtk::MessageDialog dlg( "Invalid username or password.", false, Gtk::MESSAGE_ERROR, Gtk::BUTTONS_OK, true );
	dlg.set_title( "Login Failed" );

	dlg.run();*/
//	(Gtk::Widget)(this->loginFrame).destroy();
	/*this->remove();
	
	subWindow = 0;
	subWindow = new SubWindow;
	subWindow->signal_hide().connect(sigc::mem_fun(*this, &MainWindow::closeSubWindow));
	//subWindow->show();

	this->add( gameFrame );
	this->gameFrame.startReading();
	this->gameFrame.startSubFrame(subWindow);
	
	this->show_all_children();*/
	
}
