#ifndef GAMEWINDOW_HPP
#define GAMEWINDOW_HPP


#include <gtkmm.h>


class GameWindow: public Gtk::Window {

public:	
	GameWindow();


private:
	Gtk::Grid layoutGrid;

	
	void prepareComponents();

};


#endif