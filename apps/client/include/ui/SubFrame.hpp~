#ifndef SUBFRAME_HPP
#define SUBFRAME_HPP


#include <gtkmm.h>
#include <thread>

class SubFrame: public Gtk::Frame {

public:
	SubFrame();

	void startReading();
	
private:
	Gtk::Grid layoutGrid;
	
	Gtk::ScrolledWindow scrolledWindow;
	Gtk::Box responseBox;
	Gtk::Entry commandEntry;
	Gtk::Notebook gameFrameNotebook;
	Gtk::Label outputTabLabel, inventoryTabLabel, chatTabLabel, statsTabLabel;
	
	Glib::Thread* readerThread;
	Glib::Dispatcher updateDispatcher;
	
	void prepareComponents();
	
	void readResponses_thread();
	
	void updateResponses();
	
	void enterCommand_signal();
	
};


#endif
