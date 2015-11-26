#ifndef CHARACTERFRAME_HPP
#define CHARACTERFRAME_HPP


#include <gtkmm.h>


class CharacterFrame: public Gtk::Frame {

public:
	CharacterFrame();
	
	
private:
	Gtk::Grid layoutGrid;
	Gtk::Box characterList;
	
	void setupComponents();
	
};


#endif

//m_button1.signal_clicked().connect( sigc::bind<Glib::ustring>( sigc::mem_fun(*this, &HelloWorld::on_button_clicked), "button 1") );