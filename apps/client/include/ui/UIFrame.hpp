#ifndef UIFRAME_HPP
#define UIFRAME_HPP

#include <gtkmm.h>
#include <thread>

class UIFrame: public Gtk::Frame {

public:
	UIFrame();
	
private:

	void prepareComponents();
	
	void readResponses_thread();
	
	void updateResponses();
	
	void startReading();

	void enterCommand_signal();

};


#endif
