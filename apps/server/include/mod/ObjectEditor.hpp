#ifndef ITEMEDITOR_HPP
#define ITEMEDITOR_HPP


#include <string>


class ObjectEditor {
public:
	
	static std::string createItem( int creatorId, std::string itemData );
	static std::string deleteObject( std::string itemData );
	
	static std::string placeObject( int creatorId, std::string objectData );
	
private:
	
	ObjectEditor() {}
};


#endif