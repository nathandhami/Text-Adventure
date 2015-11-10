#include <ui/LoginWindow.hpp>
#include <Locale.hpp>


#define WIDTH_DEFAULT 	900
#define HEIGHT_DEFAULT	500

#define BORDER_WIDTH_DEFAULT 10


// ------------------- PUBLIC -------------------

LoginWindow::LoginWindow() : loginButton( "Login" ) {
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

void LoginWindow::prepareComponents() {
	this->layoutGrid.set_orientation( Gtk::ORIENTATION_VERTICAL );
	this->layoutGrid.set_halign( Gtk::Align::ALIGN_CENTER );
	this->layoutGrid.set_valign( Gtk::Align::ALIGN_CENTER );
	this->layoutGrid.set_row_spacing( 10 );	

	
	this->usernameLabel.set_text( "Username:" );
//	this->usernameLabel.set_halign( Gtk::Align::ALIGN_START );
	this->passwordLabel.set_text( "Password:" );
//	this->passwordLabel.set_halign( Gtk::Align::ALIGN_START );
	this->passwordEntry.set_visibility( false );
	
	this->usernameEntry.signal_activate().connect( sigc::mem_fun( *this, &LoginWindow::loginButton_click ) );
	this->passwordEntry.signal_activate().connect( sigc::mem_fun( *this, &LoginWindow::loginButton_click ) );
	
	this->loginButton.signal_clicked().connect( sigc::mem_fun( *this, &LoginWindow::loginButton_click ) );
	
	this->layoutGrid.add( usernameLabel );
	this->layoutGrid.add( usernameEntry );
	this->layoutGrid.add( passwordLabel );
	this->layoutGrid.add( passwordEntry );
	this->layoutGrid.add( this->loginButton );
	
	this->add( layoutGrid );
	
	this->show_all_children();
}


void LoginWindow::loginButton_click() {
	
	gameWindow.show();
	
//	this->close();
	
	/*Gtk::MessageDialog dlg( "Invalid username or password.", false, Gtk::MESSAGE_ERROR, Gtk::BUTTONS_OK, true );
	dlg.set_title( "Login Failed" );

	dlg.run();*/
}