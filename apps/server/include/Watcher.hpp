#ifndef WATCHER_HPP
#define WATCHER_HPP

#include <boost/asio.hpp>
#include <boost/lexical_cast.hpp>
#include <thread>
#include <mutex>

using boost::asio::ip::tcp;

class Watcher {
public:
	Watcher();
	~Watcher();

	void run();
	bool getRunningState();
	
private:
	boost::asio::io_service ioService;
	tcp::acceptor* acceptor;
	std::thread runnerThread;
	bool running;
	std::mutex stateMutex;
	
	
	void asyncRun();
	void setRunningState( bool state );
	
};


#endif