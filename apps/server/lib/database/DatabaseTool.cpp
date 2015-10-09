#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <sqlite3.h>
#include <string>
#include "DatabaseTool.hpp"
#include "Database.h"
#include "Query.h"

using namespace std;

const string DB_LOCATION = "../../apps/server/databases/adventureDB";
const string DB_NAME = "";
const int NO_BYTE_LIMIT = -1;

// DatabaseTool::DatabaseTool() {
// 	int openDbCheck;
// 	sqlite3_config(SQLITE_CONFIG_URI, 1);
// 	openDbCheck = sqlite3_open(DB_LOCATION, &this->db);
// 	if( openDbCheck != SQLITE_OK ){
// 		throw runtime_error("Can't open database");
// 	}
// }

// DatabaseTool::~DatabaseTool() {
// 	sqlite3_close(this->db);
// }

string DatabaseTool::quotesql( const string& s ) {
    return string("'") + s + string("'");
}

bool DatabaseTool::addUser(string userID, string password) {
	Database db( DB_LOCATION );
	Query q(db);
	string sqlStatment = "INSERT INTO users VALUES (" + quotesql(userID) + "," + quotesql(password) + ");";
	q.execute(sqlStatment.c_str());


	// sqlite3_stmt * stmt;
	// string sqlStatment = "INSERT INTO users VALUES (" + quotesql(userID) + "," + quotesql(password) + ");";
	// int compileStatus = sqlite3_prepare_v2(this->db, sqlStatment.c_str(), -1, &stmt, NULL);
	// if(stmt == NULL) {
	// 	cout << sqlStatment.c_str() << endl;
	// 	cout << "could not compile, error code:" << compileStatus << endl;
	// 	return false;
	// }
	// int executeStatus = sqlite3_step(stmt);
	// if(executeStatus == SQLITE_DONE) {
	// 	return true;
	// } else {
	// 	return false;
	// }

}
