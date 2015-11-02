#include "Authenticator.hpp"
#include "DatabaseTool.hpp"

#include <boost/algorithm/string.hpp>
#include <vector>


int Authenticator::login( std::string data ) {
	std::vector< std::string > credentials;
	boost::split( credentials, data, boost::is_any_of( ";" ) );
	
	std::cout << "User's credentials: " << std::endl;
	std::cout << "\tUsername: " << credentials[ 0 ] << std::endl;
	std::cout << "\tPassword: " << credentials[ 1 ] << std::endl;
	
	int userId = DatabaseTool::getUserID( credentials[ 0 ], credentials[ 1 ] );

	if ( userId ) {
//		DatabaseTool::setCharOnline( userId ); //Has to be user in the future
		DatabaseTool::setCharOnline( userId, "stuff" );
	}
	
	return userId;
}


bool Authenticator::login( User& user, std::string data ) {
	const int NO_USER_ID = 0;
	
	std::vector< std::string > credentials;
	boost::split( credentials, data, boost::is_any_of( ";" ) );
	
	std::cout << "User's credentials: " << std::endl;
	std::cout << "\tUsername: " << credentials[ 0 ] << std::endl;
	std::cout << "\tPassword: " << credentials[ 1 ] << std::endl;
	
	user.setUserId( DatabaseTool::getUserID( credentials[ 0 ], credentials[ 1 ] ) );
	std::cout << "Logged: " << DatabaseTool::getUserID( credentials[ 0 ], credentials[ 1 ] ) << std::endl;
	
	return ( user.getUserId() != NO_USER_ID );
}


bool Authenticator::logout( User& user ) {
	const int NO_USER_ID = 0;
	
	if ( user.isAuthorized() ) {
		// TO-DO: reimplement once character selection becomes a thing
		DatabaseTool::setCharOffline( user.getUserId() );
		user.setUserId( NO_USER_ID );
		return true;
	}
	
	return false;
}


void Authenticator::logout( int userId ) {
	if ( userId ) {
		DatabaseTool::setCharOffline( userId );
	}
}
