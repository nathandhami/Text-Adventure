#ifndef GAMEFRAME_HPP
#define GAMEFRAME_HPP


#include <gtkmm.h>
#include <thread>
#include <ui/SubWindow.hpp>
#include <ui/UINotebook.hpp>

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
	Gtk::ScrolledWindow worldWindow;
	Gtk::ScrolledWindow combatWindow;
	Gtk::ScrolledWindow chatWindow;
	Gtk::ScrolledWindow statsWindow;
	Gtk::ScrolledWindow inventoryWindow;
	Gtk::Box responseBox;
	Gtk::Box worldBox;
	Gtk::Box combatBox;
	Gtk::Box chatBox;
	Gtk::Box statsBox;
	Gtk::Box inventoryBox;
	Gtk::Entry commandEntry;
	Gtk::Button commandEntryButton;
	Gtk::Label outputTabLabel;
	
	Glib::Thread* readerThread;
	Glib::Dispatcher updateDispatcher;

	UINotebook subFrameNotebook;
	UINotebook sideNotebook;
	Gtk::Label worldTabLabel, combatTabLabel, chatTabLabel;
	Gtk::Label statsTabLabel, inventoryTabLabel;
	

	void prepareComponents();
	
	void readResponses_thread();
	
	void updateResponses();
	
	void enterCommand_signal();

	std::vector<std::string> tokenizeResponses(std::string);
};


#endif
