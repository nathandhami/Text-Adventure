#ifndef SERVERCONNECTION_HPP
#define SERVERCONNECTION_HPP


using boost::asio::ip::tcp;


class ServerConnection {
public:
	ServerConnection( 
		boost::asio::io_service& ioService,
		tcp::resolver::iterator endpointIterator
	): ioService( ioService ), socket( ioService ) {}
	
	tcp::socket& getSocket() {
		return this->socket;
	}
	
private:
	tcp::socket socket;
	boost::asio::io_service& ioService;
	
	
};


#endif