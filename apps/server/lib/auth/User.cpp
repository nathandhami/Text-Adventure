#include "User.hpp"


bool User::isAuthorized() const {
	const int NO_USER_ID = 0;
	return ( this->userId != NO_USER_ID );
}


int User::getUserId() const {
	return this->userId;
}


int getSelectedCharacterId() {
	
	return 0;
}