#ifndef LOGINFRAME_HPP
#define LOGINFRAME_HPP


#include <gtkmm.h>


class LoginFrame: public Gtk::Frame {

public:
	LoginFrame();
	
	
private:
	// Frame widgets
	Gtk::Grid layoutGrid;
	Gtk::Label usernameLabel;
	Gtk::Label passwordLabel;
	Gtk::Entry usernameEntry;
	Gtk::Entry passwordEntry;
	Gtk::Button loginButton;
	Gtk::Button registerButton;

	// Widgets setup
	void setupComponents();
	void setupGrid();
	void setupButtons();
	void setupLabels();
	void setupEntries();
	
	// Action listener functions
	void loginButton_click();
	void registerButton_click();
	
};


#endif