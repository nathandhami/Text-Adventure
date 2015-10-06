#ifndef WATCHER_HPP
#define WATCHER_HPP

#include <boost/asio.hpp>
#include <boost/lexical_cast.hpp>
#include <thread>

using boost::asio::ip::tcp;

class Watcher {
public:
	Watcher();
	~Watcher();

	void run();
	
private:
	boost::asio::io_service ioService;
	tcp::acceptor* acceptor;
	bool running;
	std::mutex stateMutex;
	
};


#endif