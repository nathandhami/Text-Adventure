#ifndef GAMEFRAME_HPP
#define GAMEFRAME_HPP


#include <gtkmm.h>


class GameFrame: public Gtk::Frame {

public:
	GameFrame();
	
	
private:
	Gtk::Grid layoutGrid;
	
	Gtk::ScrolledWindow scrolledWindow;
	
	void prepareComponents();
	
	
};


#endif