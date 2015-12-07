#include <ui/CharacterFrame.hpp>
#include <ui/MainWindow.hpp>
#include "Game.hpp"

#include <boost/algorithm/string.hpp>
#include <vector>


#define UNIVERSAL_PADDING 10


// ------------------- PUBLIC -------------------

CharacterFrame::CharacterFrame() {
	this->set_name( "frame-character" );
	
	this->setupComponents();
	this->show_all_children();
}


void CharacterFrame::updateCharacterList( std::string list ) {
	// remove all chars
	this->infoBox.set_opacity( 0 );
	this->deleteButton.set_sensitive( false );
	this->selectButton.set_sensitive( false );
	
	this->charInfList.clear();
	std::vector< Gtk::Widget* > widgets = this->charContainer.get_children();
	for (  Gtk::Widget* widget: widgets ) {
		this->charContainer.remove( *widget );
	}
	
	// add new chars
	std::vector< std::string > listTokens;
	boost::split( listTokens, list, boost::is_any_of( "~" ) );
	//TO-DO: replace with regex match for multi-char delimeter
	if ( !listTokens.empty() && !listTokens[ 0 ].empty() ) {
		std::string firstSelectedName = "";
		
		for ( std::string& characterData: listTokens ) {
			if ( firstSelectedName.empty() ) {
				firstSelectedName = this->addCharacter( characterData );
			} else {
				this->addCharacter( characterData );
			}
		}
		
		
		
		
		this->selectCharacter( firstSelectedName );
		this->infoBox.set_opacity( 1 );
		this->deleteButton.set_sensitive( true );
		this->selectButton.set_sensitive( true );
	}
	
	this->addAddCharButton();
	this->show_all_children();
}


// ------------------- PRIVATE ------------------

void CharacterFrame::setupComponents() {
	this->setupLayout();
	this->setupCharList();
	this->setupButtons();
	this->setupLabels();

	this->add( layoutGrid );
}


void CharacterFrame::setupLayout() {
//	const int WIDTH = this->get_parent_window()->get_width();
//	const int HEIGHT = this->get_parent_window()->get_height();
	const int PADDING = 10;
	
	this->layoutGrid.set_row_spacing( 10 );
	this->layoutGrid.set_column_spacing( 10 );
	this->layoutGrid.set_row_homogeneous( false );
	this->layoutGrid.set_column_homogeneous( false );
	
	this->layoutGrid.set_margin_top( 10 );
	this->layoutGrid.set_margin_right( 10 );
	this->layoutGrid.set_margin_left( 10 );
	this->layoutGrid.set_margin_bottom( 10 );
	this->layoutGrid.set_size_request( -1, 480 );
}


void CharacterFrame::setupCharList() {
	this->characterList.set_size_request( 290, 440 );
	this->charContainer.set_orientation( Gtk::ORIENTATION_VERTICAL );
	this->characterList.add ( this->charContainer );
	this->layoutGrid.attach( this->characterList, 1, 0, 1, 1 );
}


void CharacterFrame::setupButtons() {
	const int BTN_WIDTH = 140;
	const int BTN_PADDING = 10;
	
	Gtk::Fixed* p_logOutButtonContainer = Gtk::manage( new Gtk::Fixed() );
	Gtk::Fixed* p_selectButtonContainer = Gtk::manage( new Gtk::Fixed() );
	p_selectButtonContainer->set_halign( Gtk::Align::ALIGN_END );
	
	// LOGOUT Button
	Gtk::Button* p_logoutButton = Gtk::manage( new Gtk::Button( "Log Out" ) );
	p_logoutButton->set_size_request( BTN_WIDTH, -1 );
	p_logoutButton->signal_clicked().connect( sigc::mem_fun( *this, &CharacterFrame::logoutButton_click ) );
	
	this->deleteButton.set_label( "Delete" );
	this->deleteButton.set_size_request( BTN_WIDTH, -1 );
	this->deleteButton.signal_clicked().connect( sigc::mem_fun( *this, &CharacterFrame::deleteButton_click ) );
	
	this->selectButton.set_label( "Enter World" );
	this->selectButton.set_size_request( BTN_WIDTH, -1 );
	this->selectButton.signal_clicked().connect( sigc::mem_fun( *this, &CharacterFrame::selectButton_click ) );
	
	p_logOutButtonContainer->put( *p_logoutButton, 0, 0 );
	p_selectButtonContainer->put( this->deleteButton, 0, 0 );
	p_selectButtonContainer->put( this->selectButton, BTN_WIDTH + BTN_PADDING, 0 );
	this->layoutGrid.attach( *p_logOutButtonContainer, 0, 1, 1, 1 );
	this->layoutGrid.attach( *p_selectButtonContainer, 1, 1, 1, 1 );
}


void CharacterFrame::setupLabels() {
	this->infoBox.set_orientation( Gtk::ORIENTATION_VERTICAL );

	this->nameLabel.set_size_request( 580, 70 );
	this->nameLabel.set_text( "Some name" );
	this->nameLabel.set_alignment( 0.0 );
	this->nameLabel.set_name( "label-character-name" );
	this->infoBox.pack_start( this->nameLabel );

	this->locationLabel.set_size_request( 580, -1 );
	this->locationLabel.set_text( "Currently in: dafuq" );
	this->locationLabel.set_alignment( 0.0 );
	this->locationLabel.set_name( "label-character-location" );
	this->infoBox.pack_start( this->locationLabel );

	this->descriptionLabel.set_size_request( 580, 300 );
	this->descriptionLabel.set_text( "What am i doing with my life?" );
	this->descriptionLabel.set_alignment( 0.0, 0.0 );
	this->descriptionLabel.set_name( "label-character-desc" );
	this->infoBox.pack_start( this->descriptionLabel );

	this->infoBox.set_name( "box-info" );
	this->layoutGrid.attach( this->infoBox, 0, 0, 1, 1 );
}


std::string CharacterFrame::addCharacter( std::string data ) {
	const int NUM_ARGS_EXP = 4;
	
	if ( data.empty() ) return "";
	
	std::vector< std::string > dataTokens;
	boost::split( dataTokens, data, boost::is_any_of( "|" ) );
	if ( dataTokens.size() != NUM_ARGS_EXP ) return "";
	
	std::string name = dataTokens[ 0 ];
	std::string level = dataTokens[ 1 ];
	std::string location = dataTokens[ 2 ];
	std::string description = dataTokens[ 3 ];
	
	this->charInfList.insert( std::pair< std::string, CharacterInfo >( name, CharacterInfo( location, description ) ) );
	this->addCharacterButton( name, level );
	
	return name;
}


void CharacterFrame::addCharacterButton( std::string charName, std::string charLevel ) {
	const int LIST_MARGIN = 5;
	
	Gtk::RadioButton* p_charButton = Gtk::manage( new Gtk::RadioButton( this->charRBGroup ) );
	p_charButton->set_mode( false );
	p_charButton->set_margin_bottom( LIST_MARGIN );
	p_charButton->set_name( "button-toggle-char" );
	
	Gtk::Grid* p_characterPane = Gtk::manage( new Gtk::Grid() );
	p_characterPane->set_name( "char-pane" );
	p_characterPane->set_row_homogeneous( false );
	p_characterPane->set_column_homogeneous( false );
	
	Gtk::Label* p_charName = Gtk::manage( new Gtk::Label( charName ) );
	p_charName->set_name( "char-name" );
	
	Gtk::Label* p_charLevel = Gtk::manage( new Gtk::Label( "level " + charLevel ) );
	Gtk::Fixed* p_lvlContainer = Gtk::manage( new Gtk::Fixed() );
	p_charLevel->set_margin_left( LIST_MARGIN );
	p_charLevel->set_margin_bottom( LIST_MARGIN );
	p_charLevel->set_name( "char-level" );
	
	p_characterPane->attach( *p_charName , 0, 0, 1, 1 );
	p_lvlContainer->add( *p_charLevel );
	p_characterPane->attach( *p_lvlContainer , 0, 1, 1, 1 );
	
	p_characterPane->set_size_request( 277, 60 );
	p_charButton->add( *p_characterPane );
	
//	p_charButton->signal_clicked().connect( sigc::mem_fun( *this, &CharacterFrame::updateCurrentChar ) );
	p_charButton->signal_clicked().connect( sigc::bind< std::string >( sigc::mem_fun( *this, &CharacterFrame::selectCharacter ), charName ) );
	
	this->charContainer.add( *p_charButton );
}


void CharacterFrame::selectCharacter( std::string name ) {
	if ( name == "" ) return;
	this->nameLabel.set_text( name );
	this->locationLabel.set_text( "Location: " + this->charInfList.find( name )->second.location );
	this->descriptionLabel.set_text( this->charInfList.find( name )->second.description );
	this->selectedCharName = name;
	
	/*std::vector< Gtk::Widget* > widgets = this->charContainer.get_children();
	for (  Gtk::Widget* widget: widgets ) {
		this->charContainer.remove( *widget );
	}
	( ( Gtk::RadioButton* )widgets[ 0 ] )->set_active();*/
}


void CharacterFrame::addAddCharButton() {
	Gtk::Button* p_addCharButton = Gtk::manage( new Gtk::Button() );
	p_addCharButton->set_size_request( 290, 60 );
	p_addCharButton->set_name( "button-add-char" );
	p_addCharButton->signal_clicked().connect( sigc::mem_fun( *this, &CharacterFrame::addCharButton_click ) );
	this->charContainer.add( *p_addCharButton );
}


void CharacterFrame::logoutButton_click() {
	Game::logout();
	MainWindow* p_parentWindow = ( MainWindow* )this->get_parent();
	if ( p_parentWindow ) {
		p_parentWindow->openLoginFrame();
	}
}


void CharacterFrame::selectButton_click() {
	NetMessage msg = Game::selectCharacter( this->selectedCharName );
	
	if ( msg.header == GameCode::OK ) {
		MainWindow* p_parentWindow = ( MainWindow* )this->get_parent();
		if ( p_parentWindow ) {
			p_parentWindow->openGameFrame();
		}
	} else {
		
	}
}


void CharacterFrame::addCharButton_click() {
	MainWindow* p_parentWindow = ( MainWindow* )this->get_parent();
	if ( p_parentWindow ) {
		p_parentWindow->openCreateFrame();
	}
}


void CharacterFrame::deleteButton_click() {
	NetMessage deleteResponse = Game::deleteCharacter( this->selectedCharName );
	
	if ( deleteResponse.header == GameCode::OK ) {
		this->updateCharacterList( deleteResponse.body );
	} else {
		Gtk::MessageDialog dlg( deleteResponse.body, false, Gtk::MESSAGE_ERROR, Gtk::BUTTONS_OK, true );
		dlg.set_decorated( false );
		dlg.set_title( "Deletion Failed" );
		dlg.run();
	}
}






