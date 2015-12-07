#ifndef GAMECODE_HPP
#define GAMECODE_HPP

#include <string>


namespace GameCode {
	// Sent by client
	// Request headers
	const std::string REGISTER 		= "reg";
	const std::string LOGIN 		= "lgn";
	const std::string LOGOUT 		= "lgo";
	const std::string CHAR_CREATE 	= "ccr";
	const std::string CHAR_DELETE 	= "cdl";
	const std::string CHAR_SELECT 	= "csl";
	const std::string CHAR_DELECT 	= "cds";
	const std::string COMMAND 		= "cmd";
	
	// Sent by server
	// Confirmation headers
	const std::string CORRECT 		= "cor";
	const std::string WRONG 		= "wrg";
	const std::string INVALID 		= "inv";
	
	// Dynamics headers
	const std::string DESCRIPTION 	= "des";
	const std::string STATUS 		= "sts";
	const std::string ALERT 		= "alr";
	const std::string COMBAT 		= "cmb";
	
	// Update headers
	const std::string INVENTORY 	= "inv";
	const std::string ATTRIBUTES 	= "atr";
	const std::string SPELLS	 	= "spl";
	
	// Chat headers
	const std::string CHAT_ZONE 	= "chz";
	const std::string CHAT_PRIVATE 	= "chp";
	
	// System headers
	const std::string DISCONNECTED 	= "dsc";
	const std::string NONE		 	= "non";
	const std::string FAUX_HEADER	= "fhd";
	const std::string OK			= "ok_";
	const std::string ERROR			= "err";

}


#endif