#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP


#include <gtkmm.h>
#include <ui/LoginFrame.hpp>
#include <ui/RegisterFrame.hpp>
#include <ui/GameFrame.hpp>

//#include <ui/GameWindow.hpp>


class MainWindow: public Gtk::Window {

public:	
	MainWindow();

	void openLoginFrame();
	void openRegisterFrame();

private:
	LoginFrame loginFrame;
	RegisterFrame registerFrame;
	GameFrame gameFrame;
	
	void setupStyleSheet();
	void setupComponents();
	
	void switchToGameFrame();
	
};


#endif