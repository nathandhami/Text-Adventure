#include <mod/Editor.hpp>
#include <mod/ObjectEditor.hpp>
#include <mod/WorldEditor.hpp>
#include "DatabaseTool.hpp"

#include <boost/algorithm/string.hpp>
#include <vector>


#define CMD_CREATE_ZONE 	"create zone"
#define CMD_DESC_ZONE 		"describe zone"
#define CMD_DELETE_ZONE		"delete zone"

#define CMD_CREATE_DOOR		"create door"
#define CMD_DELETE_DOOR		"delete door"

#define CMD_CREATE_ITEM		"create item"
#define CMD_CREATE_DOODAD	"create doodad"
#define CMD_DELETE_OBJECT	"delete object"
#define CMD_PLACE_OBJECT	"spawn object"


std::string Editor::judgeAndPerform( int creatorId, int charId, Command command ) {
	const std::string MESSAGE_INVALID = "This world is NOT your toy.";
	
	std::string editorResponse = MESSAGE_INVALID;
	
	int worthyLevel = DatabaseTool::getUserAuthencationLevel( creatorId );
	if ( worthyLevel == 0 ) {
		return MESSAGE_INVALID;
	}
	
	std::string commandType = command.type;
	std::string commandString = command.data;
	
	// WorldEditor
	if ( commandType == CMD_CREATE_ZONE ) {
		editorResponse = WorldEditor::createZone( commandString );
	} else if ( commandType == CMD_DESC_ZONE ) {
		editorResponse = WorldEditor::describeZone( creatorId, commandString );
	} else if ( commandType == CMD_DELETE_ZONE ) {
		editorResponse = WorldEditor::deleteZone( commandString );
	} else if ( commandType == CMD_CREATE_DOOR ) {
		editorResponse = WorldEditor::addDoorToZone( creatorId, commandString );
	} else if ( commandType == CMD_DELETE_DOOR ) {
		editorResponse = WorldEditor::deleteDoor( creatorId, commandString );
	} 
	// ObjectEditor
	else if ( commandType == CMD_CREATE_ITEM ) {
		editorResponse = ObjectEditor::createItem( creatorId, commandString );
	} else if ( commandType == CMD_DELETE_OBJECT ) {
		editorResponse = ObjectEditor::deleteObject( commandString );
	} else if ( commandType == CMD_PLACE_OBJECT ) {
		editorResponse = ObjectEditor::placeObject( creatorId, commandString );
	} else {
		std::cout << "What are we trying to do?\n";
	}
	
	return editorResponse;
}

const std::string Editor::REJECT_MESSAGE = "THOUGH art NOT worthyyy.";