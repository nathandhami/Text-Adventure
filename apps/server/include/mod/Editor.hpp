#ifndef EDITOR_HPP
#define EDITOR_HPP


#include "Command.hpp"


class Editor {
public:
	
	static const std::string REJECT_MESSAGE;
	
	static std::string judgeAndPerform( int creatorId, int charId, Command command );
	
	
private:
	
	Editor() {}
	
};


#endif