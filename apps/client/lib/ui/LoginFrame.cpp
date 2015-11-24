#include <ui/LoginFrame.hpp>
#include <NetMessage.hpp>
#include <GameCode.hpp>
#include "Game.hpp"


#define FORM_WIDTH 	300
#define FORM_HEIGHT 167


// ------------------- PUBLIC -------------------

LoginFrame::LoginFrame() : loginButton( "Log In" ), registerButton( "Register" ) {
	const std::string CSS_ID = "frame-login";
	
	this->set_margin_right( 1 );
	this->set_margin_left( 1 );
	this->set_margin_bottom( 1 );
	
	this->set_name( CSS_ID );

	this->setupComponents();

	this->show_all_children();
}


// ------------------- PRIVATE ------------------

void LoginFrame::setupComponents() {
	this->setupGrid();
	this->setupLabels();
	this->setupEntries();
	this->setupButtons();

	this->layoutGrid.attach( this->usernameLabel, 0, 0, 5, 1 );
	this->layoutGrid.attach( this->usernameEntry, 1, 1, 3, 1 );
	this->layoutGrid.attach( this->passwordLabel, 0, 2, 5, 1 );
	this->layoutGrid.attach( this->passwordEntry, 1, 3, 3, 1 );
	this->layoutGrid.attach( this->loginButton, 2, 4, 2, 1  );
	this->layoutGrid.attach( this->registerButton, 1, 4, 1, 1  );

	this->add( layoutGrid );
}


void LoginFrame::setupGrid() {
	this->layoutGrid.set_orientation( Gtk::ORIENTATION_VERTICAL );
	this->layoutGrid.set_halign( Gtk::Align::ALIGN_CENTER );
	this->layoutGrid.set_valign( Gtk::Align::ALIGN_CENTER );
	this->layoutGrid.set_row_spacing( 10 );
	this->layoutGrid.set_column_spacing( 10 );
	this->layoutGrid.set_size_request( 300, 187 );
}


void LoginFrame::setupLabels() {
	// Username
	this->usernameLabel.set_name( "label-username" );
	this->usernameLabel.set_markup( "<span>Username:</span>" );
	this->usernameLabel.set_size_request( 300 );
	this->usernameLabel.set_margin_top( 10 );
	
	// Password
	this->passwordLabel.set_name( "label-password" );
	this->passwordLabel.set_markup( "<span>Password:</span>" );
	this->passwordLabel.set_size_request( 300 );
}


void LoginFrame::setupEntries() {
	const int ENTRY_WIDTH = 200;
	Gdk::RGBA BG_COLOR;
	BG_COLOR.set_rgba( 0.0, 0.0, 0.0, 0.3 );
	
	// Username
	this->usernameEntry.set_alignment( 0.5 );
	this->usernameEntry.signal_activate().connect( sigc::mem_fun( *this, &LoginFrame::loginButton_click ) );
	
	// Password
	this->passwordEntry.set_alignment( 0.5 );
	this->passwordEntry.set_visibility( false );
	this->passwordEntry.signal_activate().connect( sigc::mem_fun( *this, &LoginFrame::loginButton_click ) );
}


void LoginFrame::setupButtons() {
	//	this->loginButton.set_name( "button-login" );
	this->loginButton.signal_clicked().connect( sigc::mem_fun( *this, &LoginFrame::loginButton_click ) );
}


void LoginFrame::loginButton_click() {

	std::string username = this->usernameEntry.get_text();
	std::string password = this->passwordEntry.get_text();
	
	NetMessage msg = Game::login( username, password );
	
	if ( msg.header == GameCode::CORRECT ) {
		this->hide();
	} else {
		Gtk::MessageDialog dlg( "Invalid username or password.", false, Gtk::MESSAGE_ERROR, Gtk::BUTTONS_OK, true );
		dlg.set_decorated( false );
		dlg.set_title( "Login Failed" );
		dlg.run();
	}
}