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
		DatabaseTool();
		~DatabaseTool();
		
		
	private:
		sqlite3 *db;
};

#endif