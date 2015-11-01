#ifndef GTKMM_MAINWINDOW_HPP
#define GTKMM_MAINWINDOW_HPP

#include <gtkmm.h>

class MainWindow : public Gtk::Window
{
public:
  MainWindow();
  virtual ~MainWindow();

protected:
	//Signal handlers:
	void on_enter_pressed();

  	//Child widgets:
	Glib::RefPtr<Gtk::TextBuffer> outputTextBuffer;
	Glib::RefPtr<Gtk::Adjustment> m_adjustment;
	Gtk::ScrolledWindow outputScrollWindow;
	Gtk::TextView outputTextview;
	Gtk::Grid uiGrid;
	Gtk::Entry commandEntry;
	Gtk::Box outputBox;
	Gtk::Frame commandFrame;
	Gtk::Frame outputFrame;
};

#endif
