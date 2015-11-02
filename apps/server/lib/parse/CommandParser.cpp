/*
 * CommandParser.cpp
 *
 *  This class gets the raw command from the user
 *  and parses it into the type of command it is, 
 *	and the value of the data for that command
 *  
 *  It returns a header to the controller so it knows where to send the command to,
 *  along with the type of command, and the data for the command.
 */
#include <iostream>
#include "CommandParser.hpp"
#include "NetConfig.hpp"

#define LOG( msg ) std::cout << "[Parser] " << msg << std::endl

// Handles cases such as one-wrod movements and more!
static void translateSpecial( int& commandHeader, Command& parsedCommand ) {
	const std::string MOVE_CARDINAL = "move";
	const std::string MOVE_EXCEPTION = "look";
	const std::string MOVE_SYNONYM = "go";
	
	if ( commandHeader == CommandHeader::WORLD ) {
		if ( parsedCommand.data == "" && parsedCommand.type != MOVE_EXCEPTION ) {
			parsedCommand.data = parsedCommand.type;
			parsedCommand.type = MOVE_CARDINAL;
		}
		
		if ( parsedCommand.type == MOVE_SYNONYM ) {
			parsedCommand.type = MOVE_CARDINAL;
		}
	}
}


std::tuple< int, Command > CommandParser::getHeaderAndCommand( std::string commandString ) {
	Command parsedCommand;
	int parsedHeader = INVALID;
	
	LOG( "Parsing string: '" << commandString << "'..." );
	
	std::vector< std::string > tokens;
	boost::split( tokens, commandString, boost::is_any_of( " " ), boost::token_compress_on );
	
	LOG( "Parsing tokens..." );
	
	std::string commandType = "";
	int prefixLength = 0;
	
	// copied values because of the to_lower
	for ( std::string token: tokens ) {
		boost::to_lower( token );
		LOG( "\tToken: " << token );
		
		std::string commandPrefix = commandType + " " + token;
		boost::trim( commandPrefix );
		
		LOG( "\tPrefix: " << commandPrefix );
		
		int prefixHeader = DictionaryCmds::getParsableFromInput( commandPrefix );
		LOG( "\tHeader: " << prefixHeader );
		
		if ( prefixHeader == CommandHeader::INVALID ) {
			if ( parsedHeader == CommandHeader::INCOMPLETE ) parsedHeader = CommandHeader::INVALID;
			break;
		} else {
			commandType = commandPrefix;
			parsedHeader = prefixHeader;
			prefixLength ++;
		}
	}
	
	
	
	// Get the size of 'trailing' data
	int commandDataTokenLength = tokens.size() - prefixLength;
	LOG( "Data size: " << commandDataTokenLength );
	
	// Cut off the command type from the data
	std::rotate( tokens.begin(), tokens.end() - commandDataTokenLength, tokens.end() );
	tokens.resize( commandDataTokenLength );
	
	std::string commandData = boost::algorithm::join( tokens, " " );
	
	parsedCommand.type = commandType;
	parsedCommand.data = commandData;
	
	translateSpecial( parsedHeader, parsedCommand );
	
	return std::make_tuple( parsedHeader, parsedCommand );
}



/*
int CommandParser::getHeaderAndCommand( std::string command ){

//	std::tuple<  int, Command > headerAndParsedCommand;
	int headerAndParsedCommand;
	headerAndParsedCommand = getHeaderAndCommandFromString( command );//Parse user command and get a header associated with the command type

	//returns the header type and the command 
	return headerAndParsedCommand;
}
bool CommandParser::checkIfCardinal( std::string token ){ //checks if the command was entered as a (inter)cardinal direction only (no "move" in front)
	return( (      token == "north" ) 
			  || ( token == "south" )
			  || ( token == "east" )
			  || ( token == "west" )
			  || ( token == "northeast" )
			  || ( token == "northwest" )
			  || ( token == "southeast" )
			  || ( token == "southwest" ) );
}


//takes user command in string form and parses it into a Command struct
int CommandParser::getHeaderAndCommandFromString( std::string commandString ) {
	
	Command parsedCommand;
//	std::tuple<  int, Command > headerAndParsedCommand;
	int headerAndParsedCommand;
	int commandHeader;
	std::vector< std::string > tokens;
	boost::trim(commandString);
	if(commandString == ""){
		commandHeader = INVALID;
//		headerAndParsedCommand = std::make_tuple(commandHeader, parsedCommand);
		headerAndParsedCommand = commandHeader;
		return headerAndParsedCommand;
	}


	boost::split( tokens, commandString, boost::is_any_of( " " ),boost::token_compress_on );
	for( int i =0; i<tokens.size();i++ ){//testing
		cout << tokens[i] << endl;
	}
	for(int i = 0; i<tokens.size();i++)
	{
		cout << "tokens[" << i << "] = " << tokens[i] << endl;
	}
	std::string lowerCaseToken;
	lowerCaseToken = tokens[0];
	boost::to_lower(lowerCaseToken);
	if(lowerCaseToken == "go"){
		tokens[0] = "move";
	}

	 int header;
	int startOfCommandData;
	int i = 0;
	bool isACardinal;
	bool incompleteCommand;
	bool isASingleWordCommand;



	isASingleWordCommand = ( tokens.size() == 1 );

	

	while ( header != INVALID ){
		lowerCaseToken = tokens[0];
		boost::to_lower( lowerCaseToken );
		isACardinal = checkIfCardinal( lowerCaseToken );
		header = DictionaryCmds::getParsableFromInput( lowerCaseToken );
		if(header == INCOMPLETE){ //check if it is an incomplete command
			incompleteCommand = true;
		}
		if( header != INVALID ){ //valid (possibly incomplete) command
			if( isACardinal ){ 
				parsedCommand.type = "move"; //set the command type to move if only (inter)cardinal entered
				parsedCommand.data = tokens[0];
				commandHeader = header;
				startOfCommandData = tokens.size();
				break;
			}else if (isASingleWordCommand){
				parsedCommand.type = tokens[0];
				commandHeader = header; 
				startOfCommandData = tokens.size();
				break;
			}
			else{
				parsedCommand.type = tokens[0];
				cout << "88" << endl;
				if( i+1 < tokens.size() ){//checks to make sure it is not out of bounds
				cout << "90" << endl;
					tokens[0]= tokens[0] + " " + tokens[i+1]; //concatinate the first token with the next to retest if longer command name (eg "look at")
				cout << "92" << endl;
				}else {
					commandHeader = header;
					break;
				}
			}
			//commandHeader = header;
			
		}else if( ( header == INVALID ) && ( i > 0 ) ){ //invalid header found after finding an initially valid command
			if( isACardinal ){
				startOfCommandData = 0;//data starts at beginning instead of after command since it is a cardinal direction
			}else{
				startOfCommandData = i;// data starts after the command type is found
			}
		}else if( ( header == INVALID ) && ( ( i = 0 ) || ( incompleteCommand ) ) ){
			commandHeader = header;
			startOfCommandData = tokens.size();//invalid command so data becomes as small as possible since it wont be used
		}
		i++;
	}
	if(commandHeader == INCOMPLETE){
		commandHeader = INVALID;
	}
	for ( int i = startOfCommandData; i<tokens.size();i++ ){ // concatinates data fields into one space seperated string
		if( parsedCommand.data.empty() ){
			parsedCommand.data = tokens[i];
		}else parsedCommand.data += " " + tokens[i];
	}

//	headerAndParsedCommand = std::make_tuple ( commandHeader, parsedCommand );
	headerAndParsedCommand = commandHeader;
	
	return headerAndParsedCommand;
	
}
*/
