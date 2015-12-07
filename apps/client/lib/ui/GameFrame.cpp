#include <ui/GameFrame.hpp>

#include <NetMessage.hpp>
#include <GameCode.hpp>
#include "Game.hpp"

#include <gdkmm/rgba.h>
#include <iostream>

#include <Locale.hpp>
#include <ui/SubWindow.hpp>
#include <ui/UINotebook.hpp>
#include <ui/MainWindow.hpp>

#include <string>
#include <sstream>
#include <vector>

#include <boost/algorithm/string.hpp> 

#define WIDTH_DEFAULT 	900
#define HEIGHT_DEFAULT	500

#define BORDER_WIDTH_DEFAULT 1
// ------------------- PUBLIC -------------------

GameFrame::GameFrame()
: commandEntryButton("Send"),
  logoutButton("Logout"),
  switchCharButton("Switch Char")
{
	this->prepareComponents();

	this->show_all_children();

}

// ------------------- PRIVATE ------------------

void GameFrame::prepareComponents() {
	/*this->subFrameNotebook.override_background_color( Gdk::RGBA("white"));
	this->subFrameNotebook.set_size_request( 220, 380 );
	this->subFrameNotebook.set_border_width( 5 );

	this->subFrameNotebook.append_page(inventoryTabLabel, "Inventory");
	this->subFrameNotebook.append_page(chatTabLabel, "Chat");*/

	this->layoutGrid.set_row_spacing( 10 );	
	
//	Gtk::Button* pButton = Gtk::manage(new Gtk::Button("Test"));
//	this->scrolledWindow.add(*pButton);
	
//	 Gdk::RGBA color;

	this->subFrameNotebook.set_name( "notebookColor" );
	this->sideNotebook.set_name( "notebookColor" );
	
	/*main notebook*/
	
	this->responseBox.set_orientation( Gtk::ORIENTATION_VERTICAL );
	this->responseBox.set_halign( Gtk::Align::ALIGN_START );
	this->responseBox.set_valign( Gtk::Align::ALIGN_START );
	this->responseBox.signal_size_allocate().connect( sigc::mem_fun( *this, &GameFrame::updateScrollPosition ) );

	this->worldBox.set_orientation( Gtk::ORIENTATION_VERTICAL );
	worldBox.set_halign( Gtk::Align::ALIGN_START );
	worldBox.set_valign( Gtk::Align::ALIGN_START );
	
	this->combatBox.set_orientation( Gtk::ORIENTATION_VERTICAL );
	combatBox.set_halign( Gtk::Align::ALIGN_START );
	combatBox.set_valign( Gtk::Align::ALIGN_START );
	
	this->chatBox.set_orientation( Gtk::ORIENTATION_VERTICAL );
	chatBox.set_halign( Gtk::Align::ALIGN_START );
	chatBox.set_valign( Gtk::Align::ALIGN_START );
		
	this->statsBox.set_orientation( Gtk::ORIENTATION_VERTICAL );
	statsBox.set_halign( Gtk::Align::ALIGN_START );
	statsBox.set_valign( Gtk::Align::ALIGN_START );
	
	this->inventoryBox.set_orientation( Gtk::ORIENTATION_VERTICAL );
	inventoryBox.set_halign( Gtk::Align::ALIGN_START );
	inventoryBox.set_valign( Gtk::Align::ALIGN_START );

	this->spellsBox.set_orientation( Gtk::ORIENTATION_VERTICAL );
	spellsBox.set_halign( Gtk::Align::ALIGN_START );
	spellsBox.set_valign( Gtk::Align::ALIGN_START );
	
	//this->scrolledWindow.override_background_color( Gdk::RGBA("white"));
	//this->responseBox.set_policy( Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC );
	//this->worldWindow.override_background_color( Gdk::RGBA("white"));
	//this->combatWindow.override_background_color( Gdk::RGBA("white"));
	//this->chatWindow.override_background_color( Gdk::RGBA("white"));
	//this->statsWindow.override_background_color( Gdk::RGBA("white"));
	//this->inventoryWindow.override_background_color( Gdk::RGBA("white"));
	//this->spellsWindow.override_background_color( Gdk::RGBA("white"));
	
	this->scrolledWindow.add( this->responseBox );
	this->scrolledWindow.set_size_request( 680, 380 );
	this->scrolledWindow.set_border_width( 5 );

	this->worldWindow.add( this->worldBox );
	this->worldWindow.set_size_request( 680, 380 );
	this->worldWindow.set_border_width( 5 );

	this->combatWindow.add( this->combatBox );
	this->combatWindow.set_size_request( 680, 380 );
	this->combatWindow.set_border_width( 5 );
//	this->scrolledWindow.set_valign( Gtk::Align::ALIGN_START );

	this->chatWindow.add( this->chatBox );
	this->chatWindow.set_size_request( 680, 380 );
	this->chatWindow.set_border_width( 5 );

	this->statsWindow.add( this->statsBox );
	this->statsWindow.set_size_request( 210, 110 );
	this->statsWindow.set_border_width( 5 );

	this->inventoryWindow.add( this->inventoryBox );
	this->inventoryWindow.set_size_request( 210, 110 );
	this->inventoryWindow.set_border_width( 5 );

	this->spellsWindow.add( this->spellsBox );
	this->spellsWindow.set_size_request( 210, 110 );
	this->spellsWindow.set_border_width( 5 );

	this->commandEntry.set_size_request( 670, 40 );

	this->commandEntry.signal_activate().connect( sigc::mem_fun( *this, &GameFrame::enterCommand_signal ) );
	this->commandEntryButton.signal_clicked().connect( sigc::mem_fun( *this, &GameFrame::enterCommand_signal ) );

	this->logoutButton.set_size_request( 220, 1 );
	this->logoutButton.signal_clicked().connect( sigc::mem_fun( *this, &GameFrame::logoutCommand_signal ) );
	this->switchCharButton.signal_clicked().connect( sigc::mem_fun( *this, &GameFrame::switchCommand_signal ) );	

	this->updateDispatcher.connect( sigc::mem_fun( *this, &GameFrame::updateResponses ) );

	this->subFrameNotebook.append_page(this->scrolledWindow, "All");
	this->subFrameNotebook.append_page(this->worldWindow, "World");
	this->subFrameNotebook.append_page(this->combatWindow, "Combat");
	this->subFrameNotebook.append_page(this->chatWindow, "Chat");

	this->sideNotebook.set_size_request( 220, 400 );
	//this->sideNotebook.set_border_width( 5 );
	
	this->sideNotebook.append_page(this->statsWindow, "Stats");
	this->sideNotebook.append_page(this->inventoryWindow, "Inventory");
	//this->sideNotebook.append_page(this->spellsWindow, "Spells");

	buttonGrid.attach( this->logoutButton, 0, 0, 1, 1);
	buttonGrid.attach( this->switchCharButton, 0, 1, 1, 1);
	subGrid.attach( this->sideNotebook, 0, 0, 1, 1);
	subGrid.attach( this->buttonGrid, 0, 1, 1, 1);
	
	//this->layoutGrid.add( this->subFrameNotebook );
	//this->layoutGrid.add( this->sideNotebook );
	//this->layoutGrid.add( this->commandEntry );
	//this->layoutGrid.add( this->commandEntry );
	//this->layoutGrid.attach_next_to( this->subFrameNotebook, this->commandEntry, Gtk::POS_BOTTOM, 1, 1);
	this->layoutGrid.attach( this->subFrameNotebook, 0, 0, 1, 1 );
	this->layoutGrid.attach( this->commandEntry, 0, 1, 1, 1 );
	//this->layoutGrid.attach( this->sideNotebook, 1, 0, 1, 1 );
	this->layoutGrid.attach( this->subGrid, 1, 0, 1, 1 );	
	this->layoutGrid.attach( this->commandEntryButton, 1, 1, 1, 1 );

	this->add( layoutGrid );

	this->statsLabel.set_valign( Gtk::Align::ALIGN_START );
	this->statsLabel.set_halign( Gtk::Align::ALIGN_START );
	this->statsBox.pack_start( statsLabel, Gtk::PACK_EXPAND_PADDING );

	this->inventoryLabel.set_valign( Gtk::Align::ALIGN_START );
	this->inventoryLabel.set_halign( Gtk::Align::ALIGN_START );
	this->inventoryBox.pack_start( inventoryLabel, Gtk::PACK_EXPAND_PADDING );

	this->subFrameNotebook.set_margin_left(5);
	this->subFrameNotebook.set_margin_top(5);
	this->subFrameNotebook.set_margin_right(5);
	//this->subFrameNotebook.set_margin_bottom(5);

	this->sideNotebook.set_margin_top(5);
	this->sideNotebook.set_margin_right(5);
	this->sideNotebook.set_margin_bottom(5);
	
	//this->commandEntryButton.set_margin_left(5);
	this->commandEntryButton.set_margin_right(5);
	this->commandEntryButton.set_margin_bottom(5);

	this->logoutButton.set_margin_bottom(5);

	this->commandEntry.set_margin_left(5);
	this->commandEntry.set_margin_right(5);
	this->commandEntry.set_margin_bottom(5);


//	this->show_all_children();
}

void GameFrame::updateScrollPosition( Gtk::Allocation& alloc ) {
	//std::cout << "LEL" << std::endl;
	Glib::RefPtr<Gtk::Adjustment> p_adjustment = this->scrolledWindow.get_vadjustment();
	p_adjustment->set_value( p_adjustment->get_upper() );
	this->show_all_children();
}


void GameFrame::startReading() {
	this->readerThread = Glib::Thread::create(sigc::mem_fun(*this, &GameFrame::readResponses_thread), true);

//	this->readerThread->dispatch();
}


void GameFrame::readResponses_thread() {
	while ( 1 ) {
//		std::cout << "It's done!" << std::endl;
		this->updateDispatcher.emit();
		Glib::usleep( 50000 );
	}
		
}

void GameFrame::startSubFrame(SubWindow *subWindow) {
	//subWindow->show();
}

void GameFrame::closeSubWindow() {
	
}

std::vector<std::string> GameFrame::tokenizeResponses(std::string response) {
	std::vector<std::string> tokens;
	if(response.length() > 90){
		int i = 0;
		std::string s;
		for(char c:response){
			s += c;
			if(i == 90){
				tokens.push_back(s);
				s = "";
				i = 0;
			}
			i++;
		}
		tokens.push_back(s);				
	}
	else{
		tokens.push_back(response);
	}
	return tokens;
}

void GameFrame::updateResponses() {

//	std::cout << "It's done!" << std::endl;
	NetMessage msg = Game::getFrontResponse();

	std::string response = "<span color='black'>" + msg.body + "</span>";
	//std::string combatResponse = "<span color='red'>" + msg.body + "</span>";
	
	if ( msg.header != GameCode::NONE && msg.header != GameCode::INVENTORY && msg.header != GameCode::ATTRIBUTES ) {
		Gtk::Label* pLabel = Gtk::manage( new Gtk::Label() );

		if( msg.header == GameCode::DESCRIPTION
		|| msg.header == GameCode::COMBAT
		|| msg.header == GameCode::CHAT_ZONE
		|| msg.header == GameCode::CHAT_PRIVATE
		|| msg.header == GameCode::STATUS
		|| msg.header == GameCode::INVALID ) {

			
			if(msg.header == GameCode::COMBAT) { pLabel->set_name( "combatColor" ); }
			if(msg.header == GameCode::CHAT_ZONE) { pLabel->set_name( "zoneChatColor" ); }
			if(msg.header == GameCode::CHAT_PRIVATE) { pLabel->set_name( "privateChatColor" ); }

			pLabel->set_text( msg.body );
			pLabel->set_valign( Gtk::Align::ALIGN_START );
			pLabel->set_halign( Gtk::Align::ALIGN_START );
			pLabel->set_line_wrap_mode(Pango::WrapMode::WRAP_WORD_CHAR);
			pLabel->set_line_wrap(TRUE);
			//pLabel->set_wrap_mode( Gtk::WrapMode::WRAP_WORD_CHAR );
			this->responseBox.pack_start( *pLabel, Gtk::PACK_EXPAND_PADDING );
			this->show_all_children();
		}
	}

	if ( msg.header == GameCode::DESCRIPTION
		|| msg.header == GameCode::STATUS ) {

		Gtk::Label* pLabel = Gtk::manage( new Gtk::Label() );
		pLabel->set_text( msg.body );
		pLabel->set_valign( Gtk::Align::ALIGN_START );
		pLabel->set_halign( Gtk::Align::ALIGN_START );
		pLabel->set_line_wrap_mode(Pango::WrapMode::WRAP_WORD_CHAR);
		pLabel->set_line_wrap(TRUE);
		this->worldBox.pack_start( *pLabel, Gtk::PACK_EXPAND_PADDING );
		this->show_all_children();

	}

	if ( msg.header == GameCode::COMBAT ) {

		Gtk::Label* pLabel = Gtk::manage( new Gtk::Label() );
		pLabel->set_name( "combatColor" );
		pLabel->set_text( msg.body );
		pLabel->set_valign( Gtk::Align::ALIGN_START );
		pLabel->set_halign( Gtk::Align::ALIGN_START );
		pLabel->set_line_wrap_mode(Pango::WrapMode::WRAP_WORD_CHAR);
		pLabel->set_line_wrap(TRUE);
		this->combatBox.pack_start( *pLabel, Gtk::PACK_EXPAND_PADDING );
		this->show_all_children();
	}

	if ( msg.header == GameCode::CHAT_ZONE ) {

		Gtk::Label* pLabel = Gtk::manage( new Gtk::Label() );
		pLabel->set_name( "zoneChatColor");
		pLabel->set_text( msg.body );
		pLabel->set_valign( Gtk::Align::ALIGN_START );
		pLabel->set_halign( Gtk::Align::ALIGN_START );
		pLabel->set_line_wrap_mode(Pango::WrapMode::WRAP_WORD_CHAR);
		pLabel->set_line_wrap(TRUE);
		this->chatBox.pack_start( *pLabel, Gtk::PACK_EXPAND_PADDING );
		this->show_all_children();

	}
	
	if ( msg.header == GameCode::CHAT_PRIVATE ) {

		Gtk::Label* pLabel = Gtk::manage( new Gtk::Label() );
		pLabel->set_name( "privateChatColor");
		pLabel->set_text( msg.body );
		pLabel->set_valign( Gtk::Align::ALIGN_START );
		pLabel->set_halign( Gtk::Align::ALIGN_START );
		pLabel->set_line_wrap_mode(Pango::WrapMode::WRAP_WORD_CHAR);
		pLabel->set_line_wrap(TRUE);
		this->chatBox.pack_start( *pLabel, Gtk::PACK_EXPAND_PADDING );
		this->show_all_children();

	}
	if ( msg.header == GameCode::INVENTORY ) {
		std::vector< std::string > listTokens;
		std::vector< std::string > itemsVector;
		std::vector< std::string > quantityVector;
		std::vector< std::string > equippedVector;
		std::string isEquipped, inventory;
		std::vector< std::string > inventoryVector;
		boost::split( listTokens, msg.body, boost::is_any_of( ";\n" ) );
	
		for(int i = 0; i < listTokens.size(); i++) {
				
			if(i % 3 == 0) {
				itemsVector.push_back(listTokens[i]);
			}
			
			if(i % 3 == 1) {
				quantityVector.push_back(listTokens[i]);
			}

		}

		for(int i = 2; i < listTokens.size(); i+=3) {
			if(listTokens[i] == "0") {
				equippedVector.push_back("Not Equipped");
			} else {
				equippedVector.push_back("Equipped");
			}
			
		}
	
		for(int i = 0; i < itemsVector.size(); i++) {
			inventoryVector.push_back(quantityVector[i] + "x " +
							itemsVector[i] + "\n" + 
							equippedVector[i] + "\n\n");

			inventory += inventoryVector[i];
		}
		
		/*for(int i = 0; i < inventoryVector.size(); i++) {
			inventoryLabel.set_text(inventoryVector[i]);
			//std::cout << inventoryVector[i] << std::endl;
		}*/

		inventoryLabel.set_text( inventory );
		
		//inventoryLabel.set_text(msg.body);
	
		/*this->item = listTokens[1];
		this->intStats = listTokens[3];
		this->dexStats = listTokens[5];
		this->charisStats = listTokens[7];
		this->hpStats = listTokens[9];
		this->mpStats = listTokens[11];
		this->xpStats = listTokens[13];
		this->levelStats = listTokens[15];*/

		/*Gtk::Label* pLabel = Gtk::manage( new Gtk::Label() );
		pLabel->set_text( msg.body );
		pLabel->set_valign( Gtk::Align::ALIGN_START );
		pLabel->set_halign( Gtk::Align::ALIGN_START );
		this->inventoryBox.pack_start( *pLabel, Gtk::PACK_EXPAND_PADDING );
		this->show_all_children();*/

		/*std::vector<std::string> tokens = tokenizeResponses(msg.body);
		if(tokens.size() == 0){
			Gtk::Label* pLabel = Gtk::manage( new Gtk::Label() );
			pLabel->set_text( msg.body );
			pLabel->set_valign( Gtk::Align::ALIGN_START );
			pLabel->set_halign( Gtk::Align::ALIGN_START );
			this->inventoryBox.pack_start( *pLabel, Gtk::PACK_EXPAND_PADDING );
			this->show_all_children();
		}
		else{
			
			for(std::string s : tokens){
				std::string response = "<span color='black'>" + s + "</span>";
				Gtk::Label* pLabel = Gtk::manage( new Gtk::Label() );
				pLabel->set_markup( response );
				pLabel->set_valign( Gtk::Align::ALIGN_START );
				pLabel->set_halign( Gtk::Align::ALIGN_START );
				this->inventoryBox.pack_start( *pLabel, Gtk::PACK_EXPAND_PADDING );
				this->show_all_children();
			}
				
		}*/
	}
	if ( msg.header == GameCode::ATTRIBUTES) {
		//std::cout << msg.body << std::endl;
		std::vector< std::string > listTokens;
		boost::split( listTokens, msg.body, boost::is_any_of( ":\n" ) );

		this->strStats = listTokens[1];
		this->intStats = listTokens[3];
		this->dexStats = listTokens[5];
		this->charisStats = listTokens[7];
		this->hpStats = listTokens[9];
		this->mpStats = listTokens[11];
		this->xpStats = listTokens[13];
		this->levelStats = listTokens[15];

		this->statsLabel.set_text( "Level: " + this->levelStats +
						"\nHP: " + this->hpStats + 
						"\nMP: " + this->mpStats +
						"\nXP: " + this->xpStats +
						"\nStrength: " + this->strStats +
						"\nIntellegence: " + this->intStats +
						"\nDexterity: " + this->dexStats +
						"\nCharisma: " + this->charisStats);

		/*Gtk::Label* pLabel = Gtk::manage( new Gtk::Label() );
		pLabel->set_text( "Level" + levelStats );
		pLabel->set_valign( Gtk::Align::ALIGN_START );
		pLabel->set_halign( Gtk::Align::ALIGN_START );
		this->statsBox.pack_start( *pLabel, Gtk::PACK_EXPAND_PADDING );*/
		
		/*std::vector<std::string> tokens = tokenizeResponses(msg.body);
		if(tokens.size() == 0){
			Gtk::Label* pLabel = Gtk::manage( new Gtk::Label() );
			pLabel->set_text( response );
			pLabel->set_valign( Gtk::Align::ALIGN_START );
			pLabel->set_halign( Gtk::Align::ALIGN_START );
			//this->statsBox.pack_start( *pLabel, Gtk::PACK_EXPAND_PADDING );
			this->show_all_children();
		}
		else{
			
			for(std::string s : tokens){
				std::string response = "<span color='black'>" + s + "</span>";
				Gtk::Label* pLabel = Gtk::manage( new Gtk::Label() );
				pLabel->set_text( response );
				pLabel->set_valign( Gtk::Align::ALIGN_START );
				pLabel->set_halign( Gtk::Align::ALIGN_START );
				//this->statsBox.pack_start( *pLabel, Gtk::PACK_EXPAND_PADDING );
				this->show_all_children();
			}
				
		}*/
	}

	else if ( msg.header == GameCode::SPELLS) {
		Gtk::Label* pLabel = Gtk::manage( new Gtk::Label() );
		pLabel->set_text( msg.body );
		pLabel->set_valign( Gtk::Align::ALIGN_START );
		pLabel->set_halign( Gtk::Align::ALIGN_START );
		this->spellsBox.pack_start( *pLabel, Gtk::PACK_EXPAND_PADDING );
		
		/*std::vector<std::string> tokens = tokenizeResponses(msg.body);
		if(tokens.size() == 0){
			Gtk::Label* pLabel = Gtk::manage( new Gtk::Label() );
			pLabel->set_text( response );
			pLabel->set_valign( Gtk::Align::ALIGN_START );
			pLabel->set_halign( Gtk::Align::ALIGN_START );
			//this->statsBox.pack_start( *pLabel, Gtk::PACK_EXPAND_PADDING );
			this->show_all_children();
		}
		else{
			
			for(std::string s : tokens){
				std::string response = "<span color='black'>" + s + "</span>";
				Gtk::Label* pLabel = Gtk::manage( new Gtk::Label() );
				pLabel->set_text( response );
				pLabel->set_valign( Gtk::Align::ALIGN_START );
				pLabel->set_halign( Gtk::Align::ALIGN_START );
				//this->statsBox.pack_start( *pLabel, Gtk::PACK_EXPAND_PADDING );
				this->show_all_children();
			}
				
		}*/
	}
	
	if( msg.header == GameCode::DISCONNECTED ){
		Gtk::MessageDialog dlg( "Disconnected from Server", false, Gtk::MESSAGE_ERROR, Gtk::BUTTONS_OK, true );
		dlg.set_decorated( false );
		dlg.set_title( "Error" );
		dlg.run();

		exit(1);
	}
}


void GameFrame::enterCommand_signal() {
	std::string command = commandEntry.get_text();
	Game::enact( command );
	std::cout << "Entered command:" << command << "\n";
	
	commandEntry.set_text("");
}

void GameFrame::logoutCommand_signal() {
	Gtk::MessageDialog dlg( "Are you sure you want to logout?", false, Gtk::MESSAGE_ERROR, Gtk::BUTTONS_YES_NO, true );
		dlg.set_decorated( false );
		dlg.set_title( "Warning" );
		if(dlg.run() == Gtk::RESPONSE_YES) {
			Game::logout();
			MainWindow* p_parentWindow = ( MainWindow* )this->get_parent();
			if ( p_parentWindow ) {
				p_parentWindow->openLoginFrame();
			}
		}
		//dlg.run();
}

void GameFrame::switchCommand_signal() {
	Gtk::MessageDialog dlg( "Are you sure you want to switch characters?", false, Gtk::MESSAGE_ERROR, Gtk::BUTTONS_YES_NO, true );
		dlg.set_decorated( false );
		dlg.set_title( "Warning" );
		if(dlg.run() == Gtk::RESPONSE_YES) {
			Game::deselectCurrentCharacter();
			MainWindow* p_parentWindow = ( MainWindow* )this->get_parent();
			if ( p_parentWindow ) {
				p_parentWindow->openCharacterFrame();
			}
		}
}
