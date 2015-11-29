#include <ui/GameFrame.hpp>

#include <NetMessage.hpp>
#include <GameCode.hpp>
#include "Game.hpp"

#include <gdkmm/rgba.h>
#include <iostream>

#include <Locale.hpp>
#include <ui/SubWindow.hpp>

#define WIDTH_DEFAULT 	900
#define HEIGHT_DEFAULT	500

#define BORDER_WIDTH_DEFAULT 1
// ------------------- PUBLIC -------------------

GameFrame::GameFrame(){
	

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
	
	this->responseBox.set_orientation( Gtk::ORIENTATION_VERTICAL );
	responseBox.set_halign( Gtk::Align::ALIGN_START );
	responseBox.set_valign( Gtk::Align::ALIGN_START );
	
	this->scrolledWindow.override_background_color( Gdk::RGBA("white"));
	
	this->scrolledWindow.add( this->responseBox );
	this->scrolledWindow.set_size_request( 680, 380 );
	this->scrolledWindow.set_border_width( 5 );
//	this->scrolledWindow.set_valign( Gtk::Align::ALIGN_START );
	this->commandEntry.set_size_request( 680, 40 );
	this->commandEntry.signal_activate().connect( sigc::mem_fun( *this, &GameFrame::enterCommand_signal ) );
	
	this->updateDispatcher.connect( sigc::mem_fun( *this, &GameFrame::updateResponses ) );

	this->subFrameNotebook.append_page(this->scrolledWindow, "All");
	this->subFrameNotebook.append_page(this->worldTabLabel, "World");
	this->subFrameNotebook.append_page(this->combatTabLabel, "Combat");
	this->subFrameNotebook.append_page(this->chatTabLabel, "Chat");

	this->sideNotebook.set_size_request( 220, 120 );
	//this->sideNotebook.set_border_width( 5 );
	
	this->sideNotebook.append_page(this->statsTabLabel, "Stats");
	this->sideNotebook.append_page(this->inventoryTabLabel, "Chat");
	
	//this->layoutGrid.add( this->subFrameNotebook );
	//this->layoutGrid.add( this->sideNotebook );
	//this->layoutGrid.add( this->commandEntry );
	//this->layoutGrid.add( this->commandEntry );
	//this->layoutGrid.attach_next_to( this->subFrameNotebook, this->commandEntry, Gtk::POS_BOTTOM, 1, 1);
	this->layoutGrid.attach( this->subFrameNotebook, 0, 0, 1, 1 );
	this->layoutGrid.attach( this->commandEntry, 0, 1, 1, 1 );
	this->layoutGrid.attach( this->sideNotebook, 1, 0, 1, 1 );

	this->add( layoutGrid );

//	this->show_all_children();
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

void GameFrame::updateResponses() {
//	std::cout << "It's done!" << std::endl;
	NetMessage msg = Game::getFrontResponse();
	if ( msg.header != GameCode::NONE ) {
		std::string response = "<span color='black'>" + msg.body + "</span>";
		Gtk::Label* pLabel = Gtk::manage( new Gtk::Label() );
		pLabel->set_markup( response );
		pLabel->set_valign( Gtk::Align::ALIGN_START );
		pLabel->set_halign( Gtk::Align::ALIGN_START );
		this->responseBox.pack_start( *pLabel, Gtk::PACK_EXPAND_PADDING );
		this->show_all_children();
	}
	
}


void GameFrame::enterCommand_signal() {
	std::string command = commandEntry.get_text();
	Game::enact( command );
	std::cout << "Entered command:" << command << "\n";
	
	commandEntry.set_text("");
}
