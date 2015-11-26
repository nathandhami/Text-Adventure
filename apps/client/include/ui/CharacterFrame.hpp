#ifndef CHARACTERFRAME_HPP
#define CHARACTERFRAME_HPP


#include <gtkmm.h>
#include <string>


class CharacterFrame: public Gtk::Frame {

public:
	CharacterFrame();
	
	void updateCharacterList( std::string );
	
	
private:
	Gtk::Grid layoutGrid;
	Gtk::ScrolledWindow characterList;
	
	Gtk::Box infoBox;
	Gtk::Label nameLabel;
	Gtk::Label locationLabel;
	Gtk::Label descriptionLabel;
	
	void setupComponents();
	void setupLayout();
	void setupCharList();
	
};


#endif

//m_button1.signal_clicked().connect( sigc::bind<Glib::ustring>( sigc::mem_fun(*this, &HelloWorld::on_button_clicked), "button 1") );