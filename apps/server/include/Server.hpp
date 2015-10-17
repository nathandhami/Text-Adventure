#ifndef SERVER_HPP
#define SERVER_HPP

#include <map>
#include <string>
#include <memory>

#include "Watcher.hpp"
#include "Session.hpp"


class Server {
public:
	
	typedef std::shared_ptr< Session > SessionPtr;
	typedef std::map< std::string, SessionPtr > SessionMap;
	
	static void initialize();
	
	static void start();
	
	static void waitUntilDone();
	
	static void registerNewSession( SessionPtr newSession );
	
	
	
private:
	
	static std::shared_ptr< Watcher > watcher;
	static SessionMap sessions;
	
	static std::string generateUniqueIdentifierString();
	
	
};


#endif