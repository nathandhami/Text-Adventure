#ifndef TRANSCEIVER_HPP
#define TRANSCEIVER_HPP

#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <memory>

#include "ServerConnection.hpp"


using boost::asio::ip::tcp;


class Transceiver {

public:
	Transceiver();
	~Transceiver();
	
	static Transceiver createTransceiver();
	void run();
	
	
private: 
	boost::asio::io_service ioService;
	tcp::resolver::iterator endpointIterator;
	std::shared_ptr< ServerConnection > connection;
	boost::asio::streambuf response;
//	tcp:: socket;

	void connectToHost();
	void handleConnect();
	
	void readFromHost();
	
};


#endif