#ifndef GAMECODE_HPP
#define GAMECODE_HPP

#include <string>


namespace GameCode {	
	// Request headers
	const std::string LOGIN 		= "lgn";
	const std::string LOGOUT 		= "lgo";
	const std::string CHAR_CREATE 	= "ccr";
	const std::string CHAR_DELETE 	= "cdl";
	const std::string CHAR_SELECT 	= "csl";
	const std::string CHAR_DELECT 	= "cds";
	const std::string COMMAND 		= "cmd";
	
	// Confirmation headers
	const std::string CORRECT 		= "cor";
	const std::string WRONG 		= "wrg";
	const std::string INVALID 		= "inv";
	
	// Dynamics headers
	const std::string DESCRIPTION 	= "des";
	const std::string STATUS 		= "sts";
	const std::string ALERT 		= "alr";
	const std::string COMBAT 		= "cmb";
	
	// Chat headers
	const std::string CHAT_ALL 		= "cha";
	const std::string CHAT_PRIVATE 	= "chp";
	
	// System headers
	const std::string DISCONNECTED 	= "dsc";
	const std::string NONE		 	= "non";
	const std::string FAUX_HEADER	= "fhd";

}


#endif