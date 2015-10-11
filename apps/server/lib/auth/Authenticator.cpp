#include "Authenticator.hpp"

#include <boost/algorithm/string.hpp>
#include <vector>


int Authenticator::login( std::string data ) {
	std::vector< std::string > credentials;
	boost::split( credentials, data, boost::is_any_of(";") );
	//TO-DO use Database's accessor to check credentials
	std::cout << "User's credentials: " << std::endl;
	std::cout << "\tUsername: " << credentials[0] << std::endl;
	std::cout << "\tPassword: " << credentials[1] << std::endl;
	// int userId = DatabaseTool::getUserID( credentials[ 0 ], credentials[ 1 ] );
	//END TO-DO
	return 1;
}

void Authenticator::logout( int userId ) {
	//TO-DO inform Database user is offline
}