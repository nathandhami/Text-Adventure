#ifndef CONNECTION_HPP
#define CONNECTION_HPP

//********************************************************************** 
// SESSION
// 
// This module holds information about a single connection accepted
// by the server, as well as allows asynchronous interaction between 
// the client and the world.
//
// Author: Pavel Kozlovsky (pkozlovs@sfu.ca)
//**********************************************************************


#include <queue>
#include <mutex>
#include <boost/asio.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>

#include "NetMessage.hpp"


using boost::asio::ip::tcp;

class Session {
	
public:
	enum IPType { v4, v6 };
	
	Session( boost::asio::io_service& ioService ): socket( ioService ) {}
	
	tcp::socket& getSocket();
	void start();
	
private:
	
	// Session state members
	tcp::socket socket;
	std::string clientIP_v4;
	
	// Session write queue members
	std::queue< NetMessage > responseMessageQueue;
	std::mutex messageQueueLock;
	bool writeInProgress = false;
	
	//Autherization data
	typedef struct {
		bool authorized;
		int userId;
		int characterId;
	} User;
	User currentUser;
	
	// Client interactors
	std::string read( NetMessage::MaxLength bufferLength );
	void handleRequest( std::string header, std::string body );
	
	// Deheader
	typedef void ( *ExecuteFunction )( void );
	typedef std::map< std::string, ExecuteFunction > ExecFuncMap;
	
	ExecFuncMap funcMap = {
		{ HEADER_LOGIN		, &login },
		{ HEADER_LOGOUT		, &logout },
		{ HEADER_COMMAND	, &doGameCommand }
		
	};
	
//	NetMessage stripAndExecute( std::string header, std::string body );
	
	NetMessage login( std::string credentials );
	NetMessage logout();
	NetMessage selectCharacter( std::string characterName );
	NetMessage deselectCurrentCharacter();
	NetMessage doGameCommand( std::string commandString );
	NetMessage sendMessageToCharacter( std::string charNameAndMessage );
	
	
	std::string getIP( IPType type );
	
//	void readHeader();
//	void readBody();
	
	void asyncWrite();
	bool write( std::string message );
	void writeToClient( std::string header, std::string body );
};

#endif
