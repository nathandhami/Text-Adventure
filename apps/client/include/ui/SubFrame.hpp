#ifndef SUBFRAME_HPP
#define SUBFRAME_HPP


#include <gtkmm.h>


class SubFrame: public Gtk::Frame {

public:
	SubFrame();
	
	
private:
	Gtk::Grid layoutGrid;

	Gtk::Label usernameLabel;
	Gtk::Label passwordLabel;
	Gtk::Entry usernameEntry;
	Gtk::Entry passwordEntry;

	Gtk::Button loginButton;

	void prepareComponents();
	void setupLabels();
	void setupEntries();
	
	
	
	
	void loginButton_click();
	
};


#endif
