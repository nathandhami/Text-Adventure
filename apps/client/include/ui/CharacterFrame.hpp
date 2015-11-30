#ifndef CHARACTERFRAME_HPP
#define CHARACTERFRAME_HPP


#include <gtkmm.h>
#include <string>
#include <map>


class CharacterFrame: public Gtk::Frame {

public:
	CharacterFrame();
	
	void updateCharacterList( std::string );
	
	
private:
	struct CharacterInfo {
		CharacterInfo( std::string location, std::string description ): location( location ), description( description ) {}
		std::string location;
		std::string description;
	};
	std::map< std::string, CharacterInfo > charInfList;
	std::string selectedCharName;
	
	
	Gtk::Grid layoutGrid;
	Gtk::ScrolledWindow characterList;
	Gtk::Grid charContainer;
	Gtk::RadioButton::Group charRBGroup;
	
	Gtk::Box infoBox;
	Gtk::Label nameLabel;
	Gtk::Label locationLabel;
	Gtk::Label descriptionLabel;
	
	Gtk::Button selectButton;
	Gtk::Button deleteButton;
	
	void setupComponents();
	void setupLayout();
	void setupCharList();
	void setupButtons();
	void setupLabels();
	
	void addCharacter( std::string data );
	void addCharacterButton( std::string charName, std::string charLevel );
	void addAddCharButton();
	void selectCharacter( std::string name );
	
	void logoutButton_click();
	void selectButton_click();
	void addCharButton_click();
};


#endif

//m_button1.signal_clicked().connect( sigc::bind<Glib::ustring>( sigc::mem_fun(*this, &HelloWorld::on_button_clicked), "button 1") );