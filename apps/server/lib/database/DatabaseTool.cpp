#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <sqlite3.h>
#include <string>
#include "DatabaseTool.hpp"

using namespace std;

const char * DB_LOCATION = "file:../../apps/server/databases/adventureDB";

DatabaseTool::DatabaseTool() {
	int openDbCheck;
	sqlite3_config(SQLITE_CONFIG_URI, 1);
	openDbCheck = sqlite3_open(DB_LOCATION, &this->db);
	if( openDbCheck != SQLITE_OK ){
		throw runtime_error("Can't open database");
	}
}

DatabaseTool::~DatabaseTool() {
	sqlite3_close(this->db);
}
