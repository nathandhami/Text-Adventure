#ifndef LOGINWINDOW_HPP
#define LOGINWINDOW_HPP


#include <gtkmm.h>

#include <ui/GameWindow.hpp>


class LoginWindow: public Gtk::Window {
	
public:	
	LoginWindow();
	
	
private:
	Gtk::Grid layoutGrid;
	
	Gtk::Label usernameLabel;
	Gtk::Label passwordLabel;
	Gtk::Entry usernameEntry;
	Gtk::Entry passwordEntry;
	
	Gtk::Button loginButton;
	
	void prepareComponents();
	void loginButton_click();
	
	GameWindow gameWindow;
};


#endif