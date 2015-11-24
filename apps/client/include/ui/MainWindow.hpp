#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP


#include <gtkmm.h>
#include <ui/LoginFrame.hpp>
#include <ui/GameFrame.hpp>

//#include <ui/GameWindow.hpp>


class MainWindow: public Gtk::Window {

public:	
	MainWindow();


private:
	LoginFrame loginFrame;
	GameFrame gameFrame;
	
	void setupStyleSheet();
	void setupComponents();
	
	void switchToGameFrame();
	
};


#endif