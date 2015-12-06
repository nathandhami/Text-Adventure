#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP


#include <gtkmm.h>
#include <ui/LoginFrame.hpp>
#include <ui/RegisterFrame.hpp>
#include <ui/CharacterFrame.hpp>
#include <ui/CreateFrame.hpp>
#include <ui/GameFrame.hpp>
//#include <ui/SubWindow.hpp>

//#include <ui/GameWindow.hpp>SubWindow* subWindow;


class MainWindow: public Gtk::Window {

public:	
	MainWindow();
	//SubWindow* subWindow;

	void openLoginFrame();
	void openRegisterFrame();
	void openCharacterFrame();
	void openCharacterFrame( std::string charList );
	void openCreateFrame();
	void openGameFrame();

private:
	LoginFrame loginFrame;
	RegisterFrame registerFrame;
	CharacterFrame characterFrame;
	CreateFrame createFrame;
	GameFrame gameFrame;
	
	void setupStyleSheet();
	void setupComponents();
	
	void switchToGameFrame();
	void closeSubWindow();
	
};


#endif
