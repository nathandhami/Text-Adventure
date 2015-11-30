#include <ui/RegisterFrame.hpp>
#include <ui/MainWindow.hpp>
#include <NetMessage.hpp>
#include <GameCode.hpp>
#include "Game.hpp"


#define FORM_WIDTH 	300
#define FORM_HEIGHT 167


// ------------------- PUBLIC -------------------

RegisterFrame::RegisterFrame() : cancelButton( "Cancel" ), registerButton( "Register" ) {
	const std::string CSS_ID = "frame-login";
	
	Gtk::Label* p_titleLabel = Gtk::manage( new Gtk::Label() );
	p_titleLabel->set_name( "label-title" );
	p_titleLabel->set_text( "Create a New Account" );
	p_titleLabel->set_size_request( 880, -1 );
	p_titleLabel->set_alignment( 0.0 );
	this->set_label_widget( *p_titleLabel );
	
	this->set_margin_top( 10 );
	this->set_margin_left( 10 );
	
	this->set_name( CSS_ID );

	this->setupComponents();

	this->show_all_children();
}


// ------------------- PRIVATE ------------------

void RegisterFrame::setupComponents() {
	this->setupGrid();
	this->setupLabels();
	this->setupEntries();
	this->setupButtons();

	this->layoutGrid.attach( this->usernameLabel, 0, 0, 5, 1 );
	this->layoutGrid.attach( this->usernameEntry, 1, 1, 3, 1 );
	this->layoutGrid.attach( this->passwordLabel, 0, 2, 5, 1 );
	this->layoutGrid.attach( this->passwordEntry, 1, 3, 3, 1 );
	this->layoutGrid.attach( this->passwordRepLabel, 0, 4, 5, 1 );
	this->layoutGrid.attach( this->passwordRepEntry, 1, 5, 3, 1 );
	this->layoutGrid.attach( this->cancelButton, 1, 6, 1, 1  );
	this->layoutGrid.attach( this->registerButton, 2, 6, 2, 1  );

	this->add( layoutGrid );
}


void RegisterFrame::setupGrid() {
	this->layoutGrid.set_orientation( Gtk::ORIENTATION_VERTICAL );
	this->layoutGrid.set_halign( Gtk::Align::ALIGN_CENTER );
	this->layoutGrid.set_valign( Gtk::Align::ALIGN_CENTER );
	this->layoutGrid.set_row_spacing( 10 );
	this->layoutGrid.set_column_spacing( 10 );
	this->layoutGrid.set_size_request( 300, 257 );
}


void RegisterFrame::setupLabels() {
	// Username
	this->usernameLabel.set_name( "label-username" );
	this->usernameLabel.set_markup( "<span>Username:</span>" );
	this->usernameLabel.set_size_request( 300 );
	this->usernameLabel.set_margin_top( 10 );
	
	// Password
	this->passwordLabel.set_name( "label-password" );
	this->passwordLabel.set_markup( "<span>Password:</span>" );
	this->passwordLabel.set_size_request( 300 );
	
	// Repeat password
	this->passwordRepLabel.set_name( "label-password" );
	this->passwordRepLabel.set_markup( "<span>Repeat Password:</span>" );
	this->passwordRepLabel.set_size_request( 300 );
}


void RegisterFrame::setupEntries() {
	const int ENTRY_WIDTH = 200;
	Gdk::RGBA BG_COLOR;
	BG_COLOR.set_rgba( 0.0, 0.0, 0.0, 0.3 );
	
	// Username
	this->usernameEntry.set_alignment( 0.5 );
//	this->usernameEntry.signal_activate().connect( sigc::mem_fun( *this, &RegisterFrame::loginButton_click ) );
	
	// Password
	this->passwordEntry.set_alignment( 0.5 );
	this->passwordEntry.set_visibility( false );
//	this->passwordEntry.signal_activate().connect( sigc::mem_fun( *this, &RegisterFrame::loginButton_click ) );
	
	// Repeat password
	this->passwordRepEntry.set_alignment( 0.5 );
	this->passwordRepEntry.set_visibility( false );
//	this->passwordRepEntry.signal_activate().connect( sigc::mem_fun( *this, &RegisterFrame::loginButton_click ) );
}


void RegisterFrame::setupButtons() {
	//	this->loginButton.set_name( "button-login" );
//	this->loginButton.signal_clicked().connect( sigc::mem_fun( *this, &RegisterFrame::loginButton_click ) );
	this->registerButton.signal_clicked().connect( sigc::mem_fun( *this, &RegisterFrame::registerButton_click ) );
	this->cancelButton.signal_clicked().connect( sigc::mem_fun( *this, &RegisterFrame::cancelButton_click ) );
}


void RegisterFrame::registerButton_click() {
	std::string userName = this->usernameEntry.get_text();
	std::string password = this->passwordEntry.get_text();
	std::string passwordRep = this->passwordRepEntry.get_text();
	
	NetMessage regResponse = Game::registerUser( userName, password, passwordRep );
	
	if ( regResponse.header == GameCode::OK ) {
		//TO-DO: go to character creation screen
		NetMessage loginResponse = Game::login( userName, password );
		if ( loginResponse.header == GameCode::CORRECT ) {
			MainWindow* p_parentWindow = ( MainWindow* )this->get_parent();
			if ( p_parentWindow ) {
				std::cout << "[UI] Logged: " << loginResponse.body << std::endl;
				p_parentWindow->openCharacterFrame( loginResponse.body );
				std::cout << "[UI] Logged: " << loginResponse.header << std::endl;
			}
		} else {
			Gtk::MessageDialog dlg( loginResponse.body, false, Gtk::MESSAGE_ERROR, Gtk::BUTTONS_OK, true );
			dlg.set_decorated( false );
			dlg.set_title( "Unable to Log In" );
			dlg.run();
		}
	} else {
		Gtk::MessageDialog dlg( regResponse.body, false, Gtk::MESSAGE_ERROR, Gtk::BUTTONS_OK, true );
		dlg.set_decorated( false );
		dlg.set_title( "Registration Failed" );
		dlg.run();
	}
}


void RegisterFrame::cancelButton_click() {
	auto p_parentWindow = this->get_parent();
	if ( p_parentWindow ) {
		p_parentWindow->remove( *this );
		( ( MainWindow* )p_parentWindow )->openLoginFrame();
	}
}