#ifndef SUBWINDOW_HPP
#define SUBWINDOW_HPP


#include <gtkmm.h>
#include <ui/LoginFrame.hpp>
#include <ui/GameFrame.hpp>

//#include <ui/GameWindow.hpp>


class SubWindow: public Gtk::Window {

public:	
	SubWindow();


private:
	LoginFrame loginFrame;
	GameFrame gameFrame;
	
	void prepareComponents();
	void switchToGameFrame();
	
};


#endif
