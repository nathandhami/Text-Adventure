#include <cstdlib>
#include <iostream>
#include <sqlite3.h>
#include <stdexcept>

#include "DatabaseTool.hpp" 

void addZoneTest();
void addItemTest();
void spawnItemTest();
void charOnlineTest();
void npcInstanceTest();

int main(int argc, char* argv[])
{
   try {
      cout << "running database tests" << endl;
//   	DatabaseTool::addUser("devon2", "tesaeft");

   	 cout << DatabaseTool::getUserID("devon", "test") << endl;
   	 cout << DatabaseTool::getUserID("devon2", "tesaeft") << endl;
   	 cout << DatabaseTool::getUserID("devon", "wrong password") << endl;
      // cout << DatabaseTool::addUser("test99", "testing") << endl;




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


   	// cout << DatabaseTool::getUserID("testUser1", "test1") << endl;
   	// cout << DatabaseTool::getUserID("testUser2", "test2") << endl;
   	// cout << DatabaseTool::getUserID("testUser1", "wrong password") << endl;

   	// cout << DatabaseTool::getPassword(1) << endl;
   	// cout << DatabaseTool::getPassword(2) << endl;
    //   cout << DatabaseTool::getPassword(99999) << endl;

   	// cout << DatabaseTool::addCharacter("leeeroooy jeeenkins", 1) << endl;
    //   cout << DatabaseTool::addCharacter("leeeroooy jeeenkins", 9909) << endl;



      //cout << DatabaseTool::getDirectionDesc(3054, "temple") << endl; 
   	// cout << DatabaseTool::getCharsLocation(99) << endl;

      //  cout << DatabaseTool::getZoneName(3001) << endl;
      //  cout << DatabaseTool::getZoneDesc(3001) << endl;
      // // //cout << DatabaseTool::getZoneExtendedDesc(3001) << endl;
      //  cout << DatabaseTool::getDirectionID(3001, "NORTH") << endl;
      //  cout << DatabaseTool::getDirectionDesc(3001, "NORTH") << endl;
      //  cout << DatabaseTool::getDirectionID(3001, "SOUTH") << endl;
      //  cout << DatabaseTool::getDirectionDesc(3001, "SOUTH") << endl;
      //  cout << DatabaseTool::getDirectionID(3001, "EAST") << endl;
      //  cout << DatabaseTool::getDirectionDesc(3001, "EAST") << endl;
      //  cout << DatabaseTool::getDirectionID(3001, "WEST") << endl;
      //  cout << DatabaseTool::getDirectionDesc(3001, "WEST") << endl;


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
      //addZoneTest();
      // addItemTest();
      // spawnItemTest();
      //cout << DatabaseTool::moveItem(1, Transfer::toZone, 3054);

      // cout << DatabaseTool::getCharID(1) << endl;
      // cout << DatabaseTool::getCharID(2) << endl;
      // DatabaseTool::putCharInZone(1, 3054);
      // cout << DatabaseTool::getCharsLocation(1) << endl;

      // vector<int> charsInZone = DatabaseTool::getAllOnlineCharsInZone(3054);
      // for(auto& charID: charsInZone) {
      //    cout << charID << endl;
      // }




   }
   catch(runtime_error e){
      cout << e.what() << endl;
   }
}


void npcInstanceTest() {
   DatabaseTool::placeNpcInZone(3000, 3054);
   vector<int> npcsInZone = DatabaseTool::getAllNpcsInZone(3054);
   for(auto& npcInstanceID: npcsInZone) {
      cout << DatabaseTool::getNPCDesc(DatabaseTool::getNpcIDFromInstanceID(npcInstanceID)) << endl;
   }
}

void charOnlineTest() {
   cout << DatabaseTool::isCharOnline(1) << endl;
   DatabaseTool::setCharOnline(1, "sessionID");
   cout << DatabaseTool::isCharOnline(1) << endl;
   cout << "sessionID = " << DatabaseTool::getSessionID(1) << endl;
   DatabaseTool::setCharOffline(1);
   cout << DatabaseTool::isCharOnline(1) << endl;
}

void addItemTest() {
      string longDesc = "You see a standard issue dagger here.";
      string shortDesc = "A dagger";
      vector<ExtendedDescription> extendedDescriptions;
      
      vector<string> keywords;
      keywords.push_back("dagger");

      ExtendedDescription extendedDesc("You see a dagger of great craftsmanship.  Imprinted on the side is: Merc Industries", keywords);
      extendedDescriptions.push_back(extendedDesc);
      Item dagger(3351, longDesc, shortDesc, extendedDescriptions, keywords);
      cout << DatabaseTool::addItem(dagger) << endl;
      cout << DatabaseTool::addItem(dagger) << endl;
}

void spawnItemTest() {
   cout << "spawning items..." << endl;
   cout << DatabaseTool::spawnItemInZone(3351, 3001) << endl;
   cout << DatabaseTool::spawnItemInZone(3351, 3001) << endl;
   cout << DatabaseTool::spawnItemInNpcInv(3351, 1) << endl;
   cout << DatabaseTool::spawnItemInNpcInv(3351, 1) << endl;
   cout << DatabaseTool::spawnItemInCharacterInv(3351, 1) << endl;
   cout << DatabaseTool::spawnItemInCharacterInv(3351, 1) << endl;
   cout << DatabaseTool::spawnItemInCharacterInv(3351, 9) << endl;
}

void addZoneTest() {
   vector<string> southDoorKeywords;
   southDoorKeywords.push_back("temple");
   vector<Door> doors;
   Door door1("You see the southern end of the temple", "south", southDoorKeywords, 3001);
   doors.push_back(door1);
 
   vector<string> extendedDesc1Keywords;
   extendedDesc1Keywords.push_back("altar");

   ExtendedDescription description1("Even through the altar is more than ten feet long it appears to be made from a single block of white marble.", extendedDesc1Keywords);

   vector<string> extendedDesc2Keywords;
   extendedDesc2Keywords.push_back("statue");
   extendedDesc2Keywords.push_back("odin");
   extendedDesc2Keywords.push_back("king");
   extendedDesc2Keywords.push_back("god");

   ExtendedDescription description2("The statue represents the one-eyed Odin sitting on a his throne.  He has long, grey hair and beard and a strict look on his face.  On top of the throne, just above his shoulders, his two ravens Hugin and Munin are sitting at his feet are his wolves Gere and Freke.",
    extendedDesc2Keywords);

   vector<ExtendedDescription> extendedDescriptions;
   extendedDescriptions.push_back(description1);
   extendedDescriptions.push_back(description2);

   string zoneDescription = "You are by the temple altar in the northern end of the Temple of Midgaard. A huge altar made from white polished marble is standing in front of you and behind it is a ten foot tall sitting statue of Odin, the King of the Gods.";
   string zoneName = "By the Temple Altar";
   DatabaseTool::addZone(3054,
      zoneName, 
      zoneDescription,
      extendedDescriptions,
      doors);

   cout << DatabaseTool::getZoneName(3054) << endl << endl;
   cout << DatabaseTool::getZoneDesc(3054) << endl << endl;
   cout << DatabaseTool::getZoneExtendedDesc(3054, "altar") << endl << endl;
   cout << DatabaseTool::getZoneExtendedDesc(3054, "god") << endl << endl;
   cout << DatabaseTool::getDirectionID(3054, "south") << endl << endl;
   cout << DatabaseTool::getDirectionID(3054, "blue") << endl << endl;
   cout << DatabaseTool::getDirectionDesc(3054, "south") << endl << endl;
   cout << DatabaseTool::getDirectionDesc(3054, "temple") << endl << endl;
   cout << DatabaseTool::getDirectionDesc(3054, "blue") << endl << endl;

}

