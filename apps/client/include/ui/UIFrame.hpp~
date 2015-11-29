#ifndef UINOTEBOOK_HPP
#define UINOTEBOOK_HPP

#include <gtkmm.h>
#include <thread>

class UINotebook: public Gtk::Notebook {

public:
	UINotebook();
	
private:

	void prepareComponents();
	
	void readResponses_thread();
	
	void updateResponses();
	
	void startReading();

	void enterCommand_signal();

};


#endif
