#include <ui/LoginFrame.hpp>
#include <NetMessage.hpp>
#include <GameCode.hpp>
#include "Game.hpp"


// ------------------- PUBLIC -------------------

LoginFrame::LoginFrame() : loginButton( "Login" ) {
	

	this->prepareComponents();

	this->show_all_children();
}


// ------------------- PRIVATE ------------------

void LoginFrame::prepareComponents() {
	this->layoutGrid.set_orientation( Gtk::ORIENTATION_VERTICAL );
	this->layoutGrid.set_halign( Gtk::Align::ALIGN_CENTER );
	this->layoutGrid.set_valign( Gtk::Align::ALIGN_CENTER );
	this->layoutGrid.set_row_spacing( 10 );	


	this->usernameLabel.set_text( "Username:" );
	//	this->usernameLabel.set_halign( Gtk::Align::ALIGN_START );
	this->passwordLabel.set_text( "Password:" );
	//	this->passwordLabel.set_halign( Gtk::Align::ALIGN_START );
	this->passwordEntry.set_visibility( false );

	this->usernameEntry.signal_activate().connect( sigc::mem_fun( *this, &LoginFrame::loginButton_click ) );
	this->passwordEntry.signal_activate().connect( sigc::mem_fun( *this, &LoginFrame::loginButton_click ) );

	this->loginButton.signal_clicked().connect( sigc::mem_fun( *this, &LoginFrame::loginButton_click ) );

	this->layoutGrid.add( usernameLabel );
	this->layoutGrid.add( usernameEntry );
	this->layoutGrid.add( passwordLabel );
	this->layoutGrid.add( passwordEntry );
	this->layoutGrid.add( this->loginButton );

	this->add( layoutGrid );
}


void LoginFrame::loginButton_click() {

	std::string username = this->usernameEntry.get_text();
	std::string password = this->passwordEntry.get_text();
	
	NetMessage msg = Game::login( username, password );
	
	if ( msg.header == GameCode::CORRECT ) {
		this->hide();
	} else {
		Gtk::MessageDialog dlg( "Invalid username or password.", false, Gtk::MESSAGE_ERROR, Gtk::BUTTONS_OK, true );
		dlg.set_title( "Login Failed" );
		dlg.run();
	}
}