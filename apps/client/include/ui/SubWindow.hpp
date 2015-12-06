#ifndef SUBWINDOW_HPP
#define SUBWINDOW_HPP


#include <gtkmm.h>
#include <ui/SubFrame.hpp>


class SubWindow: public Gtk::Window {

public:	
	SubWindow();


private:
	SubFrame subFrame;
	
	void prepareComponents();
	
};


#endif
