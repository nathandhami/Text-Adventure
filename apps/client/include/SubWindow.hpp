#ifndef GTKMM_SUBWINDOW_HPP
#define GTKMM_SUBWINDOW_HPP

#include <gtkmm.h>

class SubWindow : public Gtk::Window
{
public:
  SubWindow();
  virtual ~SubWindow();

protected:
	//Signal handlers:
	void on_enter_pressed();
	void get_response_thread();

	void on_login_click();

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

	Glib::Dispatcher m_Dispatcher;
  	Glib::Threads::Thread* m_WorkerThread;
	mutable Glib::Threads::Mutex m_Mutex;

	//Login Child widgets
	Gtk::Label usernameLabel;
	Gtk::Label passwordLabel;
	Gtk::Entry usernameEntry;
	Gtk::Entry passwordEntry;
	Gtk::Button loginButton;
	Gtk::Grid loginGrid;
};
#endif
