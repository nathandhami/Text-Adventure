#include <iostream>
#include <boost/regex.hpp>
#include <boost/tokenizer.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/regex.hpp>

#define LOG( msg ) std::cout << "[WorldEditor] " << msg << std::endl


typedef boost::tokenizer<boost::escaped_list_separator<std::string> > so_tokenizer;


int main() {
	
	std::string test = "[Deadman Wonderland] seen as [You see a what used to be colourful arc with giant letters spelling: 'Deadman Wonderland']";

	
//	create zone [asdasda] as [sadsadasd]
	
	boost::regex pattern( "(\\].*seen as.*\\[)" );
	std::vector< std::string > tokens;
	boost::trim_if(test, boost::is_any_of("[] ")); // could also use plain boost::trim
	
	
	boost::algorithm::split_regex( tokens, test, pattern );
	
	
	LOG( "Zone ID: " << tokens[ 0 ] );
	LOG( "Desc: " << tokens[ 1 ] );
	
	LOG( "NUM TK: " << tokens.size() );
	
	
	return 0;
}