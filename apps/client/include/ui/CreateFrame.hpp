#ifndef CREATEFRAME_HPP
#define CREATEFRAME_HPP


#include <gtkmm.h>


class CreateFrame: public Gtk::Frame {
public:
	
	CreateFrame();
	
	
private:
	
	Gtk::Grid layoutGrid;
	Gtk::Entry nameEntry;
	Gtk::TextView bioEntry;
	Gtk::Button createButton;
	Gtk::Button cancelButton;
	
	void setupComponents();
	void setupLayout();
	void setupButtons();
	void setupLabels();
	void setupEntries();

	void cancelButton_click();
	void createButton_click();
};


#endif