#ifndef CONNECTOR_HPP
#define CONNECTOR_HPP

#include <boost/array.hpp>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

class Connector {

public:
	Connector();
	~Connector();
	
	bool sendMessage( std::string message );
	std::string getLastResponseFromHost();
	
private: 
	std::string lastResponse;
	
	boost::asio::io_service ioService;
	tcp::resolver::iterator endpointIterator;
	tcp::socket* socket;
	
	void openConnectionToHost();

};


#endif