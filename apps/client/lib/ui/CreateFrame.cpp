#include <ui/CreateFrame.hpp>
#include <ui/MainWindow.hpp>


#define UNIVERSAL_PADDING 10


// ------------------- PUBLIC -------------------

CreateFrame::CreateFrame() {
	const std::string CSS_ID = "frame-create";
	
	Gtk::Label* p_titleLabel = Gtk::manage( new Gtk::Label() );
	p_titleLabel->set_name( "label-title" );
	p_titleLabel->set_text( "Create a New Character" );
	p_titleLabel->set_size_request( 880, -1 );
	p_titleLabel->set_alignment( 0.0 );
	this->set_label_widget( *p_titleLabel );

	this->set_margin_top( UNIVERSAL_PADDING );
	this->set_margin_left( UNIVERSAL_PADDING );

	this->set_name( CSS_ID );
	
	this->setupComponents();
	this->show_all_children();
}


// ------------------- PRIVATE ------------------

void CreateFrame::setupComponents() {
	this->setupLayout();
	this->setupLabels();
	this->setupEntries();
	this->setupButtons();
	
	this->add( this->layoutGrid );
}


void CreateFrame::setupLayout() {
	const int WIDTH = 600, HEIGHT = -1;
	
//	this->layoutGrid.set_orientation( Gtk::ORIENTATION_VERTICAL );
	this->layoutGrid.set_halign( Gtk::Align::ALIGN_CENTER );
	this->layoutGrid.set_valign( Gtk::Align::ALIGN_CENTER );
	this->layoutGrid.set_row_spacing( UNIVERSAL_PADDING );
	this->layoutGrid.set_column_spacing( UNIVERSAL_PADDING );
	this->layoutGrid.set_size_request( WIDTH, HEIGHT );
	this->layoutGrid.set_border_width( UNIVERSAL_PADDING );
}


void CreateFrame::setupLabels() {
	Gtk::Label* p_nameLabel = Gtk::manage( new Gtk::Label( "Name:" ) );
	p_nameLabel->set_margin_left( UNIVERSAL_PADDING );
	p_nameLabel->set_halign( Gtk::Align::ALIGN_END );
	this->layoutGrid.attach( *p_nameLabel, 0, 0, 1, 1 );
	
	Gtk::Label* p_bioLabel = Gtk::manage( new Gtk::Label( "Bio:" ) );
	this->layoutGrid.attach( *p_bioLabel, 0, 1, 1, 1 );
	p_bioLabel->set_halign( Gtk::Align::ALIGN_END );
	p_bioLabel->set_valign( Gtk::Align::ALIGN_START );
}


void CreateFrame::setupEntries() {
	this->nameEntry.set_size_request( 600, -1 );
	this->layoutGrid.attach( this->nameEntry, 1, 0, 2, 1 );
	
	Gtk::ScrolledWindow* p_bioContainer = Gtk::manage( new Gtk::ScrolledWindow() );
	p_bioContainer->set_policy( Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC );
	p_bioContainer->set_size_request( 600, 300 );
	this->bioEntry.set_wrap_mode( Gtk::WrapMode::WRAP_WORD_CHAR );
	this->bioEntry.set_hscroll_policy( Gtk::ScrollablePolicy::SCROLL_MINIMUM );
	
	this->bioEntry.set_margin_top( UNIVERSAL_PADDING );
	this->bioEntry.set_margin_left( UNIVERSAL_PADDING );
	this->bioEntry.set_margin_right( UNIVERSAL_PADDING );
	this->bioEntry.set_margin_bottom( UNIVERSAL_PADDING );
	
	p_bioContainer->add( this->bioEntry );
	this->layoutGrid.attach( *p_bioContainer, 1, 1, 2, 1 );
}


void CreateFrame::setupButtons() {
	this->cancelButton.set_label( "Back" );
	this->cancelButton.signal_clicked().connect( sigc::mem_fun( *this, &CreateFrame::cancelButton_click ) );
	this->layoutGrid.attach( this->cancelButton, 1, 2, 1, 1 );
	
	this->createButton.set_label( "Create" );
//	this->createButton.set_halign( Gtk::Align::ALIGN_END );
	this->createButton.signal_clicked().connect( sigc::mem_fun( *this, &CreateFrame::createButton_click ) );
	this->layoutGrid.attach( this->createButton, 2, 2, 1, 1 );
}


void CreateFrame::cancelButton_click() {
	MainWindow* p_parentWindow = ( MainWindow* )this->get_parent();
	if ( p_parentWindow ) {
		p_parentWindow->openCharacterFrame();
	}
}


void CreateFrame::createButton_click() {
	
}