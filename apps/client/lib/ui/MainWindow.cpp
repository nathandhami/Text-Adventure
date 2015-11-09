#include "MainWindow.hpp"
#include "Game.hpp"
#include <iostream>

MainWindow::MainWindow()
: commandFrame("Enter Command"),
  outputFrame("Server"),
  m_WorkerThread(0),
  usernameLabel("Username: "),
  passwordLabel("Password: "),
  loginButton("Login")
{	
	//intially hide the main widget
	uiGrid.hide();

	Game::initialize();
	Game::start();	

	//Sets login info
	//NetMessage response = Game::login( "testUser1", "test1" );

	//Initialize main window
	set_default_size(900, 500);
	set_title("Text Gale Online");
	set_position(Gtk::WIN_POS_CENTER);
	set_border_width(10);

	m_WorkerThread = Glib::Threads::Thread::create(sigc::mem_fun(*this, &MainWindow::get_response_thread));

	/************ LOGIN PAGE *************/
	
	loginGrid.add(usernameLabel);
	loginGrid.add(usernameEntry);
	loginGrid.add(passwordLabel);
	loginGrid.add(passwordEntry);
	loginGrid.add(loginButton);

	loginButton.signal_clicked().connect( sigc::mem_fun(*this,
              &MainWindow::on_login_click) );
	
	loginGrid.show();
	usernameLabel.show();
	usernameEntry.show();
	usernameEntry.signal_activate().connect(sigc::mem_fun(*this, &MainWindow::on_login_click));

	passwordLabel.show();
	passwordEntry.show();
	passwordEntry.signal_activate().connect(sigc::mem_fun(*this, &MainWindow::on_login_click));
    	passwordEntry.set_visibility(false);

	loginButton.show();

	/************ END OF LOGIN PAGE ******/

	outputTextview.set_editable(FALSE);
	outputScrollWindow.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
	
	//Wraps next char to new line in the textview
	outputTextview.set_wrap_mode(Gtk::WRAP_CHAR);

	//Adds the output text to scrollwindow parent
	outputScrollWindow.add(outputTextview);

	//This expands the scrollwindow vertically
	outputScrollWindow.set_vexpand(true);

	commandEntry.set_width_chars(85);
	outputBox.set_border_width(5);

	outputBox.pack_start(outputScrollWindow);

	//Adds the entry box child to its own frame
	commandFrame.add(commandEntry);
	//Adds the output box child to its own frame
	outputFrame.add(outputBox);

	uiGrid.set_orientation(Gtk::ORIENTATION_VERTICAL);
	uiGrid.add(outputFrame);
	uiGrid.add(commandFrame);
	uiGrid.add(loginGrid);

	//Adds the grid to the main window
	add(uiGrid);	

  	outputTextBuffer = Gtk::TextBuffer::create();
	outputTextBuffer->set_text("What would you like to do?");
	outputTextview.set_buffer(outputTextBuffer);

	//Enter signal to get user input and call function to handle it
	commandEntry.signal_activate().connect(sigc::mem_fun(*this, &MainWindow::on_enter_pressed));

	outputTextBuffer->insert(outputTextBuffer->end(), "\n");
	
	//Sets the focus on the command entry box when the program starts
	set_focus_child(commandEntry);

	uiGrid.show();

	//show_all_children();
}

MainWindow::~MainWindow()
{
}

void MainWindow::on_login_click()
{
	std::string username = usernameEntry.get_text();
	std::string password = passwordEntry.get_text();
	//NetMessage response = Game::login( "testUser1", "test1" );
	NetMessage response = Game::login( username, password );

	if(response.header == GameCode::CORRECT) {
		
		usernameLabel.hide();
		usernameEntry.hide();
		passwordLabel.hide();
		passwordEntry.hide();
		loginButton.hide();

		outputScrollWindow.show();
		outputTextview.show();
		uiGrid.show();
		commandEntry.show();
		outputBox.show();
		commandFrame.show();
		outputFrame.show();

	} else {
		Gtk::MessageDialog dlg("Invalid username or password.", false, Gtk::MESSAGE_ERROR, Gtk::BUTTONS_OK, true);
		
       		dlg.set_title("Login Failed");
		
    		dlg.run();
	}

}

void MainWindow::on_enter_pressed()
{
	std::string command = commandEntry.get_text();
	Game::enact(command);

	commandEntry.set_text("");
}

void MainWindow::get_response_thread()
{
	Glib::Threads::Mutex::Lock lock(m_Mutex);
	while(true) {
		NetMessage response = Game::getFrontResponse();
		m_adjustment = outputScrollWindow.get_vadjustment();
		m_adjustment->set_value(m_adjustment->get_upper());
		
		if ( response.header == GameCode::DISCONNECTED ) {

			exit(1);			
			
		}
	
		if ( response.header != GameCode::NONE ) {
			outputTextBuffer->insert(outputTextBuffer->end(), ">> " + response.body + "\n");
		}

		outputTextview.set_buffer(outputTextBuffer);

		Glib::usleep(500000); // microseconds
	}
}
