#include <ui/GameFrame.hpp>

#include <gdkmm/rgba.h>

#include <iostream>

// ------------------- PUBLIC -------------------

GameFrame::GameFrame() {
	

	this->prepareComponents();

	this->show_all_children();
}


// ------------------- PRIVATE ------------------

void GameFrame::prepareComponents() {
//	this->layoutGrid.set_orientation( Gtk::ORIENTATION_VERTICAL );
//	this->layoutGrid.set_halign( Gtk::Align::ALIGN_CENTER );
//	this->layoutGrid.set_valign( Gtk::Align::ALIGN_CENTER );
	this->layoutGrid.set_row_spacing( 10 );	
	
//	Gtk::Button* pButton = Gtk::manage(new Gtk::Button("Test"));
//	this->scrolledWindow.add(*pButton);
	
	 Gdk::RGBA color;
	
	
//	this->scrolledWindow.override_background_color( Gdk::RGBA("red"), GTK_STATE_FLAG_NORMAL);
	
//	Gtk::Label* pLabel = Gtk::manage(new Gtk::Label());
//	pLabel->set_markup("<b><i><span color='blue'>Open Popup</span></i></b>");
//	this->scrolledWindow.add(*pLabel);
	
	this->scrolledWindow.set_size_request( 880, 380 );
	this->scrolledWindow.set_border_width( 5 );
//	this->scrolledWindow.set_valign( Gtk::Align::ALIGN_START );
	
	this->layoutGrid.add( this->scrolledWindow );
	

	this->add( layoutGrid );

//	this->show_all_children();
}


/*void GameFrame::loginButton_click() {

}*/