#ifndef EDITOR_HPP
#define EDITOR_HPP


#include "Command.hpp"


class Editor {
public:
	
	static bool judgeAndPerform( int creatorId, Command command );
	
	
private:
	
	Editor() {}
	
};


#endif