#ifndef EDITOR_HPP
#define EDITOR_HPP


#include "Command.hpp"


class Editor {
public:
	
	static const std::string REJECT_MESSAGE;
	
	static bool judgeAndPerform( int creatorId, int charId, Command command );
	
	
private:
	
	Editor() {}
	
};


#endif