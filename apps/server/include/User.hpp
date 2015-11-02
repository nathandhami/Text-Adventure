#ifndef USER_HPP
#define USER_HPP


class User {
public:
	
	User() {}
	
	bool isAuthorized() const;
	
	void setUserId( int userId );
	int getUserId() const;
		
	void setSelectedCharacterId( int characterId );
	
	
private:
	
	int userId = 0;
	int selectedCharacterId = 0;
	
};


#endif