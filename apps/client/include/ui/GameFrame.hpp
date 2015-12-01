#ifndef GAMEFRAME_HPP
#define GAMEFRAME_HPP


#include <gtkmm.h>
#include <thread>
#include <ui/SubWindow.hpp>

class GameFrame: public Gtk::Frame {

public:
	GameFrame();
	
	void startReading();
	void startSubFrame(SubWindow *subWindow);
	void closeSubWindow();
	
private:
	Gtk::Grid layoutGrid;
	Gtk::Grid subGrid;
	Gtk::ScrolledWindow subWindow;
	
	Gtk::ScrolledWindow scrolledWindow;
	Gtk::Box responseBox;
	Gtk::Entry commandEntry;
	Gtk::Button commandEntryButton;
	Gtk::Label outputTabLabel;
	
	Glib::Thread* readerThread;
	Glib::Dispatcher updateDispatcher;

	Gtk::Notebook subFrameNotebook;
	Gtk::Notebook sideNotebook;
	Gtk::Label worldTabLabel, combatTabLabel, chatTabLabel;
	Gtk::Label statsTabLabel, inventoryTabLabel;
	

	void prepareComponents();
	
	void readResponses_thread();
	
	void updateResponses();
	
	void enterCommand_signal();

	std::vector<std::string> tokenizeResponses(std::string);
};


#endif
