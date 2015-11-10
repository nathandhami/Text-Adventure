#ifndef LOGINFRAME_HPP
#define LOGINFRAME_HPP


#include <gtkmm.h>


class LoginFrame: public Gtk::Frame {

public:
	LoginFrame();
	
	
private:
	Gtk::Grid layoutGrid;

	Gtk::Label usernameLabel;
	Gtk::Label passwordLabel;
	Gtk::Entry usernameEntry;
	Gtk::Entry passwordEntry;

	Gtk::Button loginButton;

	void prepareComponents();
	void loginButton_click();
	
};


#endif