#ifndef UINOTEBOOK_HPP
#define UINOTEBOOK_HPP

#include <gtkmm.h>
#include <thread>

class UINotebook: public Gtk::Notebook {

public:
	UINotebook(){}
	
private:
	void refreshTab(std::string Tab);

};


#endif
