#ifndef CHARACTERFRAME_HPP
#define CHARACTERFRAME_HPP


#include <gtkmm.h>


class CharacterFrame: public Gtk::Frame {

public:
	CharacterFrame();
	
	
private:
	Gtk::Grid layoutGrid;
	
	void setupComponents();
	
};


#endif