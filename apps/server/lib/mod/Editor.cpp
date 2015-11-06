#include <mod/Editor.hpp>
#include <mod/ObjectEditor.hpp>
#include <mod/WorldEditor.hpp>
#include "DatabaseTool.hpp"

#include <boost/algorithm/string.hpp>
#include <vector>


#define CMD_CREATE_ZONE 	"create zone"
#define CMD_DESC_ZONE 		"describe zone"
#define CMD_CREATE_DOOR		"create door in"
#define CMD_CREATE_ITEM		"create item"
#define CMD_CREATE_DOODAD	"create doodad"


std::string Editor::judgeAndPerform( int creatorId, int charId, Command command ) {
	const std::string MESSAGE_INVALID = "This world is NOT your toy.";
	
	//TO-DO: confirm user is a creator
	// Server::sendMessageToCharacter( creatorId, GameCode::ALERT, formattedMessage );
	// return false;
	

	
//	std::string commandString = "Deadman Wonderland >> You see a what used to be colourful arc with giant letters spelling: 'Deadman Wonderland' >> It's also known as hell, prisoners go here to enjoy their last days in deadly competetive games for food and water.";
	
//	std::string commandString = "Deadman Wonderland | You see a what used to be colourful arc with giant letters spelling: 'Deadman Wonderland'";
//	std::string commandString = "4 ~as It's also known as hell. Prisoners go here to enjoy their last days in deadly competetive games for food and water. ~: bloodmagic, sexyoverwatch, totallynotrippedoff";
//	std::string commandString = "4 ~at north ~to 4 ~: You see the entrance to the prison. ~:";
	
//	std::string commandString = "a standard issue dagger ~: You see a standard issue dagger here~; You see a dagger of great craftsmanship. Imprinted on the side is: 'Merc Industries'~; dagger, merc";
	
	std::string editorResponse = MESSAGE_INVALID;
	
	int worthyLevel = DatabaseTool::getUserAuthencationLevel( creatorId );
	if ( worthyLevel == 0 ) {
		return Editor::REJECT_MESSAGE;
	}
	
	std::string commandType = command.type;
	std::string commandString = command.data;
	
	
	if ( commandType == CMD_CREATE_ZONE ) {
		editorResponse = WorldEditor::createZone( commandString );
	} else if ( commandType == CMD_DESC_ZONE ) {
		editorResponse = WorldEditor::describeZone( creatorId, commandString );
	} else if ( commandType == CMD_CREATE_DOOR ) {
		WorldEditor::addDoorToZone( creatorId, commandString );
	} else if ( commandType == CMD_CREATE_ITEM ) {
		ObjectEditor::createItem( creatorId, commandString );
	}else {
		std::cout << "What are we trying to do?\n";
	}
	
	return editorResponse;
}

const std::string Editor::REJECT_MESSAGE = "THOUGH art NOT worthyyy.";