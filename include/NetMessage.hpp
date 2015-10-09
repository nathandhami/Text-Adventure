#ifndef NETMESSAGE_HPP
#define NETMESSAGE_HPP

class NetMessage {
public:
    enum MaxLength { HEADER = 4, BODY = 512 };
    
    NetMessage(){}
    ~NetMessage(){}
	
	std::string getBody() {
		return this->bodyLine;
	}
    
    void copyToBuffer( char* buffer, std::size_t length ) {
		this->bodyLine = std::string( buffer, length );
	}
    
    void setBodyLength( std::size_t length ) {
        this->bodyLength = length;
    }
    
    std::size_t getBodyLength() {
        return this->bodyLength;
    }
    
private:
    char bodyBuffer[512];
    char headerBuffer[4];
	
	std::string bodyLine;
    
    size_t bodyLength;
};



#endif