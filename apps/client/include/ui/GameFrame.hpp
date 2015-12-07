#ifndef GAMEFRAME_HPP
#define GAMEFRAME_HPP


#include <gtkmm.h>
#include <thread>


class GameFrame: public Gtk::Frame {

public:
	GameFrame();
	
	void startReading();
	
	
private:
	Gtk::Grid layoutGrid;
	
	Gtk::ScrolledWindow scrolledWindow;
	Gtk::Box responseBox;
	Gtk::Entry commandEntry;
	
	Glib::Thread* readerThread;
	Glib::Dispatcher updateDispatcher;
	
	void prepareComponents();
	
	void readResponses_thread();
	
	void updateResponses();
	
	void enterCommand_signal();
};


#endif