// #include "DatabaseTool.hpp"
// #include <sqlite3.h>
// #include <string>



// DatabaseTool::DatabaseTool() {
// 	int rc = sqlite3_open("../../databases/adventureDB", &this->db);
// 	if( rc ){
// 		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(this->db));
// 		exit(0);
// 	}else{
// 		fprintf(stderr, "Opened database successfully\n");
// 	}

// }


// DatabaseTool::~DatabaseTool() {
// 	sqlite3_close(this->db);
// }

