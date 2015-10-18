#ifndef NETMESSAGE_HPP
#define NETMESSAGE_HPP

class NetMessage {
public:
    enum MaxLength { HEADER = 3, BODY = 512 };
    
    NetMessage(){}
    ~NetMessage(){}
	
	
	std::string getHeader() {
		return this->headerLine;
	}
	
	std::string getBody() {
		return this->bodyLine;
	}
	
	
	//Store actual strings
	void saveHeaderString( std::string string ) {
		this->headerLine = string;
	}
	
	void saveBodyString( std::string string ) {
		this->bodyLine = string;
	}
	
    
	//Conver char* buffers to strings and store
    void saveHeaderBuffer( const char* buffer ) {
		this->headerLine = std::string( buffer, MaxLength::HEADER );
	}
	
	void saveBodyBuffer( const char* buffer, std::size_t length ) {
		this->bodyLine = std::string( buffer, length );
	}
	
    
private:
	std::string headerLine;
	std::string bodyLine;
};



#endif