#ifndef REGISTERFRAME_HPP
#define REGISTERFRAME_HPP


#include <gtkmm.h>


class RegisterFrame: public Gtk::Frame {

public:
	RegisterFrame();
	
	
private:
	// Frame widgets
	Gtk::Grid layoutGrid;
	
	Gtk::Label usernameLabel;
	Gtk::Label passwordLabel;
	Gtk::Label passwordRepLabel;
	
	Gtk::Entry usernameEntry;
	Gtk::Entry passwordEntry;
	Gtk::Entry passwordRepEntry;
	
	Gtk::Button registerButton;
	Gtk::Button cancelButton;

	// Widgets setup
	void setupComponents();
	void setupGrid();
	void setupButtons();
	void setupLabels();
	void setupEntries();
	
	// Action listener functions
	void cancelButton_click();
	void registerButton_click();
	
};


#endif