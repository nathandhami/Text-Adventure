#ifndef DATABASE
#define DATABASE


#include <string>
#include <stdexcept>
#include <iostream>
#include <sqlite3.h>
#include <fstream>

using namespace std;

class DatabaseTool{
	public:
		 static bool addUser(string userID, string password);
		
	private:
		// sqlite3 *db;
		static string quotesql( const string& s );
};

#endif