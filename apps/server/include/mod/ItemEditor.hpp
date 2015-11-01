#ifndef ITEMEDITOR_HPP
#define ITEMEDITOR_HPP


#include <string>


class ItemEditor() {
public:
	
	static void createItem( int creatorId, std::string itemData );
	
private:
	
	ItemEditor() {}
}


#endif