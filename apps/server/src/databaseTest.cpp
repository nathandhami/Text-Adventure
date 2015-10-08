#include <cstdlib>
#include <iostream>
#include <sqlite3.h> 

int main(int argc, char* argv[])
{
   sqlite3 *db;
   char *zErrMsg = 0;
   int rc;
   sqlite3_config(SQLITE_CONFIG_URI, 1);

   rc = sqlite3_open("file:../../apps/server/databases/adventureDB", &db);

   if( rc ){
      fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
      return(1);
   }else{
      fprintf(stderr, "Opened database successfully\n");
   }
   sqlite3_close(db);
}