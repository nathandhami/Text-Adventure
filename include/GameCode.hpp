#ifndef GAMECODE_HPP
#define GAMECODE_HPP

#include <string>

enum GameCode {
	DESC, ALERT, COMBAT, STATUS, PRIV_CHAT, GLBL_CHAT, DC, WRONG, CORRECT
};

class GameCode {
	
	// Confirmation headers
	static const std::string CORRECT 		= "cor";
	static const std::string WRONG 			= "wrg";
	static const std::string INVALID 		= "inv";
	
	// Dynamics headers
	static const std::string DESCRIPTION 	= "des";
	static const std::string STATUS 		= "sts";
	static const std::string ALERT 			= "alr";
	static const std::string COMBAT 		= "cmb";
	
	// Chat headers
	static const std::string CHAT_ALL 		= "cha";
	static const std::string CHAT_PRIVATE 	= "chp";
	
	// System headers
	static const std::string DISCONNECTED 	= "dsc";
		
private:
	GameCode() {}
}


#endif