#ifndef SERVERCONNECTION_HPP
#define SERVERCONNECTION_HPP

//********************************************************************** 
// SERVER CONNECTION
// 
// A simplified version of server's session adapted to the client side
// Serves as a long life-span container for the socket.
//
// Author: Pavel Kozlovsky (pkozlovs@sfu.ca)
//**********************************************************************


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
