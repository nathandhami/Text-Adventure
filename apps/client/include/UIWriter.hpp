#ifndef UIWRITER_HPP
#define UIWRITER_HPP

#include <iostream>



namespace UIWriter {
	
	void sendNotification( std::string message );
	
	void sendSysStatus( std::string message );
	
	void sendAlert( std::string message );
	
	void sendDescription( std::string message );
	
}


#endif