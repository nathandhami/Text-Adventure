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
    
    void saveHeaderBuffer( char* buffer ) {
		this->headerLine = std::string( buffer, MaxLength::HEADER );
	}
	
	void saveBodyBuffer( char* buffer, std::size_t length ) {
		this->bodyLine = std::string( buffer, length );
	}
    
    void setBodyLength( std::size_t length ) {
        this->bodyLength = length;
    }
    
    std::size_t getBodyLength() {
        return this->bodyLength;
    }
    
private:
	std::string headerLine;
	std::string bodyLine;
    
    size_t bodyLength;
};



#endif