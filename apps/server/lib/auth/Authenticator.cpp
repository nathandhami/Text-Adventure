#include "Authenticator.hpp"
#include "DatabaseTool.hpp"

#include <boost/algorithm/string.hpp>
#include <vector>


int Authenticator::login( std::string data ) {
	std::vector< std::string > credentials;
	boost::split( credentials, data, boost::is_any_of( ";" ) );
	
	std::cout << "User's credentials: " << std::endl;
	std::cout << "\tUsername: " << credentials[0] << std::endl;
	std::cout << "\tPassword: " << credentials[1] << std::endl;
	
	int userId = DatabaseTool::getUserID( credentials[ 0 ], credentials[ 1 ] );
	if ( userId ) {
		DatabaseTool::setCharOnline( userId ); //Has to be user in the future
	}
	return userId;
}

void Authenticator::logout( int userId ) {
	if ( userId ) {
		DatabaseTool::setCharOffline( userId );
	}
}