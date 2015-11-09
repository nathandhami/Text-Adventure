#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP


#include <gtkmm.h>
#include <ui/LoginFrame.hpp>

//#include <ui/GameWindow.hpp>


class MainWindow: public Gtk::Window {

public:	
	MainWindow();


private:
	LoginFrame loginFrame;
	
	void prepareComponents();
	void switchToGameFrame();
	
};


#endif