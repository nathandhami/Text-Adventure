#include <cstdlib>
#include <iostream>
#include <sqlite3.h>
#include <stdexcept>

#include "DatabaseTool.hpp" 

int main(int argc, char* argv[])
{
   try {

     DatabaseTool::addCharacter("leeroy jenkins", "devon");
   	cout << DatabaseTool::getCharsLocation(1) << endl;
   	DatabaseTool::putCharInZone(1, 8888);
   	cout << DatabaseTool::getCharsLocation(1) << endl;

   }
   catch(runtime_error e){
      cout << e.what() << endl;
   }
}