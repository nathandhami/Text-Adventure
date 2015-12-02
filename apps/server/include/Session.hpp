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
#include <atomic>
#include <thread>
#include <boost/asio.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>

#include "NetConfig.hpp"
#include "GameCode.hpp"
#include "NetMessage.hpp"
#include "User.hpp"


using boost::asio::ip::tcp;

class Session {
	
public:
	enum IPType { v4, v6 };
	
	Session( boost::asio::io_service& ioService ): socket( ioService ) {}
	~Session();
	
	tcp::socket& getSocket();
	void start( std::string identifierString );
	
	void writeToClient( std::string header, std::string body );
	
	
private:
	
	// Session state members
	tcp::socket socket;
	std::string clientIP_v4;
	std::string identifierString;
	bool terminating = false;
	
	// threads
	std::thread readerThread;
	std::thread writerThread;
	
	std::atomic< bool > writing { false };
	std::atomic< bool > reading { false };
	
	// Session write queue members
	std::queue< NetMessage > responseMessageQueue;
	std::mutex messageQueueLock;
	bool writeInProgress = false;
	int requestCounter;
	
	//Autherization data
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
		{ GameCode::REGISTER	, &Session::registerUser },
		{ GameCode::LOGIN		, &Session::login },
		{ GameCode::LOGOUT		, &Session::logout },
		{ GameCode::CHAR_CREATE	, &Session::createCharacter },
		{ GameCode::CHAR_DELETE	, &Session::deleteCharacter },
		{ GameCode::CHAR_SELECT	, &Session::selectCharacter },
		{ GameCode::COMMAND		, &Session::doGameCommand }
	};
	
//	NetMessage stripAndExecute( std::string header, std::string body );
	
	void registerUser( const std::string& credentials );
	void login( const std::string& credentials );
	void logout( const std::string& placeholder );
	
	void createCharacter( const std::string& charData );
	void deleteCharacter( const std::string& charName );
	void selectCharacter( const std::string& characterName );
	void deselectCurrentCharacter( const std::string& placeholder );
	void doGameCommand( const std::string& commandString );
	
	
	std::string getIP( IPType type );
	void gentleShutDown();
};

#endif
