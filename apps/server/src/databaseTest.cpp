#include <cstdlib>
#include <iostream>
#include <sqlite3.h>
#include <stdexcept>

#include "DatabaseTool.hpp" 

int main(int argc, char* argv[])
{
   try {
      DatabaseTool tool;
      cout << "opened database" << endl;
      return(1);


   }
   catch(runtime_error e){
      cout << e.what() << endl;
   }
}