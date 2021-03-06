#include <ui/SubFrame.hpp>
#include <NetMessage.hpp>
#include <GameCode.hpp>
#include "Game.hpp"

#include <gdkmm/rgba.h>
#include <iostream>


#define FORM_WIDTH 	300
#define FORM_HEIGHT 167


// ------------------- PUBLIC -------------------

SubFrame::SubFrame() {

	this->prepareComponents();

	this->show_all_children();
}


// ------------------- PRIVATE ------------------

void SubFrame::prepareComponents() {
//	this->layoutGrid.set_orientation( Gtk::ORIENTATION_VERTICAL );
//	this->layoutGrid.set_halign( Gtk::Align::ALIGN_CENTER );
//	this->layoutGrid.set_valign( Gtk::Align::ALIGN_CENTER );
	this->layoutGrid.set_row_spacing( 10 );	
	
//	Gtk::Button* pButton = Gtk::manage(new Gtk::Button("Test"));
//	this->scrolledWindow.add(*pButton);
	
//	 Gdk::RGBA color;
	
	this->responseBox.set_orientation( Gtk::ORIENTATION_VERTICAL );
	responseBox.set_halign( Gtk::Align::ALIGN_START );
	responseBox.set_valign( Gtk::Align::ALIGN_START );
	
	this->scrolledWindow.override_background_color( Gdk::RGBA("white"));
	
	
	
	
	this->scrolledWindow.add( this->responseBox );
	this->scrolledWindow.set_size_request( 380, 380 );
	this->scrolledWindow.set_border_width( 5 );
//	this->scrolledWindow.set_valign( Gtk::Align::ALIGN_START );
	this->commandEntry.set_size_request( 380, 40 );
	this->commandEntry.signal_activate().connect( sigc::mem_fun( *this, &SubFrame::enterCommand_signal ) );
	
	this->updateDispatcher.connect( sigc::mem_fun( *this, &SubFrame::updateResponses ) );
	
	this->layoutGrid.add( this->scrolledWindow );
//	this->layoutGrid.add( this->commandEntry );
	this->layoutGrid.attach_next_to( this->commandEntry, this->scrolledWindow, Gtk::POS_BOTTOM, 1, 1);

//	this->add( layoutGrid );

// ------------- NOTEBOOK ---------------------

	this->gameFrameNotebook.append_page(statsTabLabel, "Stats");
	this->gameFrameNotebook.append_page(inventoryTabLabel, "Inventory");
	this->gameFrameNotebook.append_page(chatTabLabel, "Chat");
	this->add( gameFrameNotebook);

//	this->show_all_children();
}


void SubFrame::startReading() {
	this->readerThread = Glib::Thread::create(sigc::mem_fun(*this, &SubFrame::readResponses_thread), true);
//	this->readerThread->dispatch();
}


void SubFrame::readResponses_thread() {
	while ( 1 ) {
//		std::cout << "It's done!" << std::endl;
		this->updateDispatcher.emit();
		Glib::usleep( 50000 );
	}
		
}


void SubFrame::updateResponses() {
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


void SubFrame::enterCommand_signal() {
	std::string command = commandEntry.get_text();
	Game::enact( command );
	std::cout << "Entered command:" << command << "\n";
	
	commandEntry.set_text("");
}
