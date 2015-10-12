#include <cstdlib>
#include <iostream>
#include <sqlite3.h>
#include <stdexcept>

#include "DatabaseTool.hpp" 

int main(int argc, char* argv[])
{
   try {
      cout << "running database tests" << endl;
   	//DatabaseTool::addUser("devon2", "tesaeft");

   	// cout << DatabaseTool::getUserID("devon", "test") << endl;
   	// cout << DatabaseTool::getUserID("devon2", "tesaeft") << endl;
   	// cout << DatabaseTool::getUserID("devon", "wrong password") << endl;

   	// cout << DatabaseTool::getPassword(1) << endl;
   	// cout << DatabaseTool::getPassword(2) << endl;

   	//DatabaseTool::addCharacter("leeeroooy jeeenkins", 1);
   	//cout << "adding leroy" << endl;

      // cout << DatabaseTool::isCharOnline(1) << endl;
      // DatabaseTool::setCharOnline(1);
      // cout << DatabaseTool::isCharOnline(1) << endl;
      // DatabaseTool::setCharOffline(1);
      // cout << DatabaseTool::isCharOnline(1) << endl;
  

   	// cout << DatabaseTool::getCharsLocation(99) << endl;
   	//DatabaseTool::putCharInZone(2, 8888);
   	// cout << DatabaseTool::getCharsLocation(1) << endl;

      // cout << DatabaseTool::getZoneName(3001) << endl;
      // cout << DatabaseTool::getZoneDesc(3001) << endl;
      // //cout << DatabaseTool::getZoneExtendedDesc(3001) << endl;
      // cout << DatabaseTool::getDirectionID(3001, "NORTH") << endl;
      // cout << DatabaseTool::getDirectionDesc(3001, "NORTH") << endl; 
      // cout << DatabaseTool::getDirectionID(3001, "SOUTH") << endl;
      // cout << DatabaseTool::getDirectionDesc(3001, "SOUTH") << endl;
      // cout << DatabaseTool::getDirectionID(3001, "EAST") << endl;
      // cout << DatabaseTool::getDirectionDesc(3001, "EAST") << endl;
      // cout << DatabaseTool::getDirectionID(3001, "WEST") << endl;
      // cout << DatabaseTool::getDirectionDesc(3001, "WEST") << endl;

      // vector<int> charsinzone = DatabaseTool::getAllCharsInZone(8888);
      // for(int i = 0; i < charsinzone.size(); i++) {
      //    cout << charsinzone[i] << endl;
      // }

      // vector<vector<string>> extendedDescs = DatabaseTool::getZoneExtendedDesc(3054);
      // cout << extendedDescs.size() << endl;

      // for(int i = 0; i < extendedDescs.size(); i++) {
      //    cout << extendedDescs[i].size() << endl;
      //    for(int x = 0; x < extendedDescs[i].size();  x++) {
      //       cout << extendedDescs[i][x] << endl << endl;
      //    }
      // }

      cout<< DatabaseTool::getZoneExtendedDesc(3054, "odin") << endl;

      //cout << DatabaseTool::getZoneExtendedDesc(3054) << endl;

      // DatabaseTool::addZone(1234, "testZone", "", "", 0, "", 0, "", 0, "", 0, "", 0, "", 0, "");
      // cout << DatabaseTool::getZoneName(1234) << endl;
      // cout << DatabaseTool::getZoneDesc(1234) << endl;
      // cout << DatabaseTool::getDirectionID(1234, "UP") << endl;


   }
   catch(runtime_error e){
      cout << e.what() << endl;
   }
}