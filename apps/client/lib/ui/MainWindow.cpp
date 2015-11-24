#include <ui/MainWindow.hpp>
#include <Locale.hpp>


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

	this->set_name( "main-window" );
	
//	this->set_border_width( BORDER_WIDTH_DEFAULT );

	this->setupStyleSheet();
	this->setupComponents();

//	this->show_all_children();
}




void MainWindow::openLoginFrame() {
	this->add( this->loginFrame );
	this->show_all_children();
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
	
	
//	this->add( this->loginFrame );
//	this->loginFrame.show();
	this->add( this->registerFrame );
	this->registerFrame.show();
	
}


void MainWindow::switchToGameFrame() {
	/*Gtk::MessageDialog dlg( "Invalid username or password.", false, Gtk::MESSAGE_ERROR, Gtk::BUTTONS_OK, true );
	dlg.set_title( "Login Failed" );

	dlg.run();*/
//	(Gtk::Widget)(this->loginFrame).destroy();
	this->remove();
	this->add( gameFrame );
	this->gameFrame.startReading();
	
	this->show_all_children();
	
}