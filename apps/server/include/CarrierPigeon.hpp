#ifndef CARRIERPIGEON_HPP
#define CARRIERPIGEON_HPP

#include <string>


class CarrierPigeon {
public:
	
	static int deliverPackage( int senderId/*, Command messageCommand*/ );
	
	
private:
	CarrierPigeon() {}
	
	static int deliverToZone( int senderId, std::string message );
	static int deliverToCharacter( int senderId, std::string message );
	
};


#endif