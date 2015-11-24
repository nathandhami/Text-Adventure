#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP


#include <gtkmm.h>
#include <ui/LoginFrame.hpp>
#include <ui/GameFrame.hpp>
//#include <ui/SubWindow.hpp>

//#include <ui/GameWindow.hpp>SubWindow* subWindow;


class MainWindow: public Gtk::Window {

public:	
	MainWindow();
	SubWindow* subWindow;

private:
	LoginFrame loginFrame;
	GameFrame gameFrame;
	
	void prepareComponents();
	void switchToGameFrame();
	void closeSubWindow();
	
};


#endif
