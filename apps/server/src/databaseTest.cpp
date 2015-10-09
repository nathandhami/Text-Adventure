#include <cstdlib>
#include <iostream>
#include <sqlite3.h>
#include <stdexcept>

#include "DatabaseTool.hpp" 

int main(int argc, char* argv[])
{
   try {
   	//DatabaseTool::addUser("devon2", "tesaeft");

   	// cout << DatabaseTool::getUserID("devon", "test") << endl;
   	// cout << DatabaseTool::getUserID("devon2", "tesaeft") << endl;
   	// cout << DatabaseTool::getUserID("devon", "wrong password") << endl;

   	// cout << DatabaseTool::getPassword(1) << endl;
   	// cout << DatabaseTool::getPassword(2) << endl;

   	DatabaseTool::addCharacter("leeeroooy jeeenkins", 1);
   	cout << "adding leroy" << endl;

    //  DatabaseTool::addCharacter("leeroy jenkins", 1);
   	// cout << DatabaseTool::getCharsLocation(1) << endl;
   	// DatabaseTool::putCharInZone(1, 8888);
   	// cout << DatabaseTool::getCharsLocation(1) << endl;

   }
   catch(runtime_error e){
      cout << e.what() << endl;
   }
}