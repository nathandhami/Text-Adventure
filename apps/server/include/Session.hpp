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
#include <thread>
#include <boost/asio.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>

#include "NetConfig.hpp"
#include "NetMessage.hpp"


using boost::asio::ip::tcp;

class Session {
	
public:
	enum IPType { v4, v6 };
	
	Session( boost::asio::io_service& ioService ): socket( ioService ) {}
	
	tcp::socket& getSocket();
	void start();
	
	void writeToClient( std::string header, std::string body );
	
	
private:
	
	// Session state members
	tcp::socket socket;
	std::string clientIP_v4;
	
	// theads
	std::thread readerThread;
	std::thread writerThread;
	
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
	void asyncReadUserRequest();
	std::string read( int bufferLength );
	void handleRequest( std::string header, std::string body );
	
	void asyncWrite();
	bool write( std::string message );
	
	// Deheader
	typedef void ( Session::*ExecuteFunction )( const std::string& );
	typedef std::map< std::string, ExecuteFunction > ExecFuncMap;
	
	ExecFuncMap funcMap = {
		{ HEADER_LOGIN		, &Session::login },
		{ HEADER_LOGOUT		, &Session::logout },
		{ HEADER_COMMAND	, &Session::doGameCommand },
		{ HEADER_MESSAGE	, &Session::sendMessageToCharacter }
	};
	
//	NetMessage stripAndExecute( std::string header, std::string body );
	
	void login( const std::string& credentials );
	void logout( const std::string& placeholder );
	void selectCharacter( const std::string& characterName );
	void deselectCurrentCharacter( const std::string& placeholder );
	void doGameCommand( const std::string& commandString );
	void sendMessageToCharacter( const std::string& charNameAndMessage );
	
	
	std::string getIP( IPType type );
};

#endif
