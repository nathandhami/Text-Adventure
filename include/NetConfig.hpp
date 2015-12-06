#ifndef NETCONFIG_HPP
#define NETCONFIG_HPP

// Server IP address and port number
#define HOST_ADDRESS_IP4 	"127.0.0.1"
//#define HOST_ADDRESS_IP4 	"10.0.1.19"
#define HOST_PORT 			9999

// Header types for network messaging
#define HEADER_LOGIN 	"lgn"
#define HEADER_LOGOUT	"lgo"
#define HEADER_COMMAND	"cmd"
#define HEADER_MESSAGE	"msg"

#define HEADER_OK		"okk"
#define HEADER_ERROR	"err"


#endif