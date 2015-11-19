#include <ui/SubWindow.hpp>
#include <Locale.hpp>


#define WIDTH_DEFAULT 	900
#define HEIGHT_DEFAULT	100

#define BORDER_WIDTH_DEFAULT 1


// ------------------- PUBLIC -------------------

SubWindow::SubWindow() {
	this->set_title( GameLocale::TITLE );

	this->set_position( Gtk::WIN_POS_CENTER );
	this->set_default_size( WIDTH_DEFAULT, HEIGHT_DEFAULT );
	this->set_size_request( WIDTH_DEFAULT, HEIGHT_DEFAULT );
	this->set_resizable( false );

	this->set_name( "sub-window" );
	
//	this->set_border_width( BORDER_WIDTH_DEFAULT );

	this->prepareComponents();

//	this->show_all_children();
}


// ------------------- PRIVATE ------------------

void SubWindow::prepareComponents() {
	
	this->loginFrame.signal_hide().connect( sigc::mem_fun( *this, &SubWindow::switchToGameFrame ) );
	
	
	this->add( this->loginFrame );
	this->loginFrame.show();
	
}


void SubWindow::switchToGameFrame() {
	/*Gtk::MessageDialog dlg( "Invalid username or password.", false, Gtk::MESSAGE_ERROR, Gtk::BUTTONS_OK, true );
	dlg.set_title( "Login Failed" );

	dlg.run();*/
//	(Gtk::Widget)(this->loginFrame).destroy();
	this->remove();
	this->add( gameFrame );
	this->gameFrame.startReading();
	
	this->show_all_children();
	
}
