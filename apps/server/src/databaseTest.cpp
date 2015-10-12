#include <cstdlib>
#include <iostream>
#include <sqlite3.h>
#include <stdexcept>

#include "DatabaseTool.hpp" 

int main(int argc, char* argv[])
{
   try {
      cout << "running database tests" << endl;
      if(DatabaseTool::addUser("devon", "testing")){
         cout << "true" << endl;
      } else {
         cout << "false" << endl;
      }

      // DatabaseTool::addNPC(3000, 
      //    " The wizard looks old and senile, and yet he looks like a very powerful wizard. He is equipped with fine clothing, and is wearing many fine rings and bracelets.", 
      //    "wizard",
      //    "A wizard walks around behind the counter, talking to himself.",
      //    "the wizard");
      // cout << DatabaseTool::getNPCDesc(3000) << endl;

      // DatabaseTool::placeNpcInZone(3000, 3054);
      // vector<int> npcsinzone = DatabaseTool::getAllNpcsInZone(3054);
      // for(int i = 0; i < npcsinzone.size(); i++) {
      //    cout << npcsinzone[i] << endl;
      // }
      //DatabaseTool::removeNpcFromZone(3000, 3054);


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

      // for(int i = 0; i < extendedDescs.size(); i++) {
      //    cout << extendedDescs[i].size() << endl;
      //    for(int x = 0; x < extendedDescs[i].size();  x++) {
      //       cout << extendedDescs[i][x] << endl << endl;
      //    }
      // }

      //cout<< DatabaseTool::getZoneExtendedDesc(3054, "odin") << endl;

      //cout << DatabaseTool::getZoneExtendedDesc(3054) << endl;

      //DatabaseTool::addZone(1234, "testZone", "", "", 0, "", 0, "", 0, "", 0, "", 0, "", 0, "");
      // cout << DatabaseTool::getZoneName(1234) << endl;
      // cout << DatabaseTool::getZoneDesc(1234) << endl;
      // cout << DatabaseTool::getDirectionID(1234, "UP") << endl;

      // DatabaseTool::addZone(3105,
      //    "Park Entrance",
      //    " You are standing just inside the small park of Midgaard.  To the north is the promenade and a small path leads south into the park. To your east is the famous Park Cafe.",
      //    "  - desc:\nThe fresh young leaves of the elm tree wave gently in the wind.\nkeywords:\n- elm\n- tree",
      //    0,
      //    "",
      //    0,
      //    "",
      //    0,
      //    "",
      //    0,
      //    "",
      //    0,
      //    "",
      //    0,
      //    "");

   }
   catch(runtime_error e){
      cout << e.what() << endl;
   }
}