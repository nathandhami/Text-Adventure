#include <ui/CharacterFrame.hpp>


// ------------------- PUBLIC -------------------

CharacterFrame::CharacterFrame() {
	this->set_name( "frame-character" );
	
	this->setupComponents();
	this->show_all_children();
}


void CharacterFrame::updateCharacterList( std::string list ) {
	
}


// ------------------- PRIVATE ------------------

void CharacterFrame::setupComponents() {
	this->setupLayout();
	this->setupCharList();
	
	this->infoBox.set_orientation( Gtk::ORIENTATION_VERTICAL );
	
	this->nameLabel.set_size_request( 580, 70 );
	this->nameLabel.set_text( "Some name" );
	this->nameLabel.set_alignment( 0.0 );
	this->nameLabel.set_name( "label-character-name" );
	this->nameLabel.set_margin_bottom( 10 );
	this->infoBox.pack_start( this->nameLabel );
	
	this->locationLabel.set_size_request( 580, -1 );
	this->locationLabel.set_text( "Currently in: dafuq" );
	this->locationLabel.set_alignment( 0.0 );
	this->locationLabel.set_margin_bottom( 10 );
	this->locationLabel.set_name( "label-character-location" );
	this->infoBox.pack_start( this->locationLabel );
	
	this->descriptionLabel.set_size_request( 580, 300 );
	this->descriptionLabel.set_text( "What am i doing with my life?" );
	this->descriptionLabel.set_alignment( 0.0, 0.0 );
	this->infoBox.pack_start( this->descriptionLabel );
	
	this->layoutGrid.attach( this->infoBox, 0, 0, 1, 1 );
	
	this->layoutGrid.attach( this->characterList, 1, 0, 1, 1 );
	this->add( layoutGrid );
}


void CharacterFrame::setupLayout() {
//	const int WIDTH = this->get_parent_window()->get_width();
//	const int HEIGHT = this->get_parent_window()->get_height();
	const int PADDING = 10;
	
	this->layoutGrid.set_row_spacing( 10 );
	this->layoutGrid.set_column_spacing( 10 );
	this->layoutGrid.set_row_homogeneous( false );
	this->layoutGrid.set_column_homogeneous( false );
	
	this->layoutGrid.set_margin_top( 10 );
	this->layoutGrid.set_margin_right( 10 );
	this->layoutGrid.set_margin_left( 10 );
	this->layoutGrid.set_margin_bottom( 10 );
//	this->layoutGrid.set_size_request( 300, 187 );
}


void CharacterFrame::setupCharList() {
	this->characterList.set_size_request( 290, 400 );
//	this->characterList.set_halign( Gtk::Align::ALIGN_CENTER );
}