#include <ui/GameWindow.hpp>
#include <Locale.hpp>


#define WIDTH_DEFAULT 	900
#define HEIGHT_DEFAULT	500

#define BORDER_WIDTH_DEFAULT 10


// ------------------- PUBLIC -------------------

GameWindow::GameWindow() {
	this->set_title( GameLocale::TITLE );

	this->set_position( Gtk::WIN_POS_CENTER );
	this->set_default_size( WIDTH_DEFAULT, HEIGHT_DEFAULT );
	this->set_size_request( WIDTH_DEFAULT, HEIGHT_DEFAULT );
	this->set_resizable( false );

	this->set_border_width( BORDER_WIDTH_DEFAULT );

	this->prepareComponents();

	this->show_all_children();
}


// ------------------- PRIVATE ------------------

void GameWindow::prepareComponents() {
	

	this->add( layoutGrid );
	
	this->show_all_children();
}