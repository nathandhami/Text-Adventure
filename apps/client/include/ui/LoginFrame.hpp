#ifndef LOGINFRAME_HPP
#define LOGINFRAME_HPP


#include <gtkmm.h>


class LoginFrame: public Gtk::Frame {

public:
	LoginFrame();
	
	
private:
	Gtk::Grid layoutGrid;
//	Gtk::Box loginForm;

	Gtk::Label usernameLabel;
	Gtk::Label passwordLabel;
	Gtk::Entry usernameEntry;
	Gtk::Entry passwordEntry;

	Gtk::Button loginButton;

	void setupComponents();
	
	void setupGrid();
	void setupLabels();
	void setupEntries();
	
	
	
	
	void loginButton_click();
	
};


#endif