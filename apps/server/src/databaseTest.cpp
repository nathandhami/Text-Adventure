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
void updateAttributesTest();
void inventoryTest();
void lookTest();

int main(int argc, char* argv[])
{
   try {
      cout << "running database tests" << endl;
      //cout << DatabaseTool::addUser("testUser1", "test1") << endl;

      // cout << DatabaseTool::getUserAuthencationLevel(1) << endl;
      // cout << DatabaseTool::setUserAuthencationLevel(1, 99) << endl;
      // cout << DatabaseTool::getUserAuthencationLevel(1) << endl;

      // cout << DatabaseTool::getUserID("testUser1", "test1") << endl;
      // cout << DatabaseTool::getUserID("testUser2", "test2") << endl;
      // cout << DatabaseTool::getUserID("testUser1", "wrong password") << endl;

      // cout << DatabaseTool::getPassword(1) << endl;
      // cout << DatabaseTool::getPassword(2) << endl;
    //   cout << DatabaseTool::getPassword(99999) << endl;

      // cout << DatabaseTool::addCharacter("leeeroooy jeeenkins", 1, "human male") << endl;
    //   cout << DatabaseTool::addCharacter("leeeroooy jeeenkins", 9909, "human male") << endl;



      //cout << DatabaseTool::getDirectionDesc(3054, "temple") << endl; 
      // cout << DatabaseTool::getCharsLocation(99) << endl;


      // cout << DatabaseTool::getZoneName(3001) << endl;
      // cout << DatabaseTool::getZoneDesc(3001) << endl;
      // cout << DatabaseTool::getDirectionID(3054, "south") << endl;


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
      //addItemTest();
      //spawnItemTest();

      // vector<int> charsInZone = DatabaseTool::getAllOnlineCharsInZone(3054);
      // for(auto& charID: charsInZone) {
      //    cout << charID << endl;
      // }

      // vector<string> usersCharacters = DatabaseTool::getCharactersNames(1);
      // for(auto& name: usersCharacters) {
      //    cout << name << endl;
      // }

      // cout << DatabaseTool::getCharIDFromName("testChar1") << endl;

      // ResetCommand command("npc", 3068, 0, 2, 3040, "", 0);
      // ResetCommand command2("npc", 3068, 0, 2, 3001, "", 0);
      // cout << DatabaseTool::addResetCommand(command) << endl;
      // cout << DatabaseTool::addResetCommand(command2) << endl;
      // cout << DatabaseTool::addResetCommand(command2) << endl;
      

      //cout << DatabaseTool::getCharNameFromID(1) << endl;;
      //updateAttributesTest();
      //npcInstanceTest();
      //inventoryTest();

      //cout << DatabaseTool::equipItem(1, "sword") << endl;

      // cout << DatabaseTool::pickUp(1, "dagger") << endl;
      // cout << DatabaseTool::pickUp(1, "sword") << endl;
      // cout << DatabaseTool::pickUp(1, "chest") << endl;

      // cout << DatabaseTool::inCombat(1, Target::character) << endl;
      // cout << DatabaseTool::setCombatFlag(1, true, Target::character) << endl;
      // cout << DatabaseTool::inCombat(1, Target::character) << endl;
      // cout << DatabaseTool::setCombatFlag(1, false, Target::character) << endl;
      // cout << DatabaseTool::inCombat(1, Target::character) << endl << endl;
      

      // cout << DatabaseTool::inCombat(1, Target::npc) << endl;
      // cout << DatabaseTool::setCombatFlag(1, true, Target::npc) << endl;
      // cout << DatabaseTool::inCombat(1, Target::npc) << endl;
      // cout << DatabaseTool::setCombatFlag(1, false, Target::npc) << endl;
      // cout << DatabaseTool::inCombat(1, Target::npc) << endl;

      // cout << DatabaseTool::getCharIDInZoneFromName("leroy", 3001) << endl;
      // cout << DatabaseTool::getCharIDInZoneFromName("leroy", 3054) << endl;

    
      // cout << DatabaseTool::getNpcInstanceIDFromName("wizaragfafar", 3001) << endl;
      // cout << DatabaseTool::getNpcInstanceIDFromName("wizard", 3054) << endl;
      // cout << DatabaseTool::getNpcInstanceIDFromName("wizard", 3001) << endl;        
  
      lookTest();
      //DatabaseTool::executeCommands();
   }
   catch(runtime_error e){
      cout << e.what() << endl;
   }
}

void lookTest() {

   cout << DatabaseTool::look(1, "inventory") << endl << endl;
   // cout << DatabaseTool::look(1, "testChar1") << endl;
}

void inventoryTest() {
   vector<string> itemNames = DatabaseTool::getItemsInZone(3054);
   for(auto& item: itemNames) {
      cout << item << endl;
   }
}

void updateAttributesTest() {
   Attributes attributes = DatabaseTool::getAttributes(1, Target::npc);
   attributes.print();
   attributes.level = 99;
   DatabaseTool::updateAttributes(attributes, Target::npc);

}

void npcInstanceTest() {
   vector<string> keywords;
   keywords.push_back("wizard");
   DatabaseTool::addNpc(3000, 
       " The wizard looks old and senile, and yet he looks like a very powerful wizard. He is equipped with fine clothing, and is wearing many fine rings and bracelets.", 
      keywords,
      "A wizard walks around behind the counter, talking to himself.",
      "the wizard");

   DatabaseTool::createNpcInstance(3000, 3054);
   vector<int> npcsInZone = DatabaseTool::getAllAliveNpcsInZone(3054);
   for(auto& npcInstanceID: npcsInZone) {
      cout << DatabaseTool::getNpcDesc(npcInstanceID) << endl << endl;;
   }

   cout << DatabaseTool::isNpcAlive(1) <<  endl;
   DatabaseTool::murderNpc(1);
   cout << DatabaseTool::isNpcAlive(1) << endl;
   DatabaseTool::reviveNpc(1);
   cout << DatabaseTool::isNpcAlive(1) << endl;
   DatabaseTool::murderNpc(1);
   cout << DatabaseTool::isNpcAlive(1) << endl;
   DatabaseTool::respawnAll();
   cout << DatabaseTool::isNpcAlive(1) << endl;

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
      
      vector<string> keywords;
      keywords.push_back("dagger");

      string desc("You see a dagger of great craftsmanship.  Imprinted on the side is: Merc Industries");

      Item dagger(9999, longDesc, shortDesc, desc, keywords);
      cout << DatabaseTool::addItem(dagger) << endl;
      cout << DatabaseTool::addItem(dagger) << endl;
}

void spawnItemTest() {
   cout << "spawning items..." << endl;
   cout << DatabaseTool::spawnItemInZone(1, 3054) << endl;
   cout << DatabaseTool::spawnItemInZone(2, 3054) << endl;
   cout << DatabaseTool::spawnItemInNpcInv(1, 1) << endl;
   cout << DatabaseTool::spawnItemInNpcInv(2, 1) << endl;
   cout << DatabaseTool::spawnItemInCharacterInv(1, 1) << endl;
   cout << DatabaseTool::spawnItemInCharacterInv(2, 1) << endl;
   cout << DatabaseTool::spawnItemInItem(1, 1) << endl;
   cout << DatabaseTool::spawnItemInItem(1, 2) << endl;
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
   cout << "adding zone= " << DatabaseTool::addZone(3054,
      zoneName, 
      zoneDescription,
      extendedDescriptions,
      doors) << endl;
   cout << "getting zone info:" << endl;
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