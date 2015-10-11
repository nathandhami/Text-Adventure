#include <iostream>
#include <string>

class DictionaryCmds {
	
	public:
		DictionaryCmds();
		~DictionaryCmds(); // destructor 
		int dictionarySize(); // returns number of commands in dictionary
		bool isInDictionaryCmds(); // checks if a command exists in the dictionary

	private: 
		int numOfCmds;
		//map<string,int> commands;
};