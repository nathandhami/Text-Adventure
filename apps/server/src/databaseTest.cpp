#include <cstdlib>
#include <iostream>
#include <sqlite3.h>
#include <stdexcept>

#include "DatabaseTool.hpp" 

int main(int argc, char* argv[])
{
   try {

      DatabaseTool::addUser("devon", "testingaslfdn");

   }
   catch(runtime_error e){
      cout << e.what() << endl;
   }
}