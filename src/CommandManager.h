#pragma once
/* Project */
#include "ControlsInputPayload.h"
//#include "Command.h"
#include "SystemManagerBase.h"
#include "EntityStates.h"
/* STL */
#include <vector>
//#include <memory>
#include <functional>

/* Routes commands to Entities. Owned by the Game State. */
class CommandManager : public SystemManagerBase
{
public:
	CommandManager();

	/* Input data sent by the Controls Manager every frame*/
	std::vector<ControlsInputPayload> FrameInputBuffer;

	/* Process the inputs*/
	std::tuple<EntityEvent, EntityEvent> processFrameInput_V2();
	EntityEvent resolveMovementInputs();
	EntityEvent resolveActionInputs();


	/* Change Controlled Entity */
	//void setPlayerControlledEntity(Entity* NewControlledEntityRef);

	/* System Manager Base Virtual Inheritance */
	virtual void receiveFromCentralDispatch(std::any reqData, const eDataType& reqDataType);
	virtual eManagerType viewMyManagerType();
	virtual std::any searchForData(const eDataType& reqDataType, const std::string& reqDataName);

private:
	//Entity* ControlledEntity{};

	///* Command Objects w/ default behaviors */
	//// Keyboard
	//std::unique_ptr<Command> InputKeyW{ std::make_unique<CommandMoveUp>() };
	//std::unique_ptr<Command> InputKeyA{ std::make_unique<CommandMoveLeft>() };
	//std::unique_ptr<Command> InputKeyS{ std::make_unique<CommandMoveDown>() };
	//std::unique_ptr<Command> InputKeyD{ std::make_unique<CommandMoveRight>() };

	//std::unique_ptr<Command> InputKeyWA{ std::make_unique<CommandMoveUpLeft>() };
	//std::unique_ptr<Command> InputKeyWD{ std::make_unique<CommandMoveUpRight>() };
	//std::unique_ptr<Command> InputKeySA{ std::make_unique<CommandMoveDownLeft>() };
	//std::unique_ptr<Command> InputKeySD{ std::make_unique<CommandMoveDownRight>() };

	//std::unique_ptr<Command> InputKey_WASDReleased{ std::make_unique<CommandMoveIdle>() };
	//std::unique_ptr<Command> InputKey_NoActions{ std::make_unique<CommandActionIdle>() };

	//std::unique_ptr<Command> InputSpaceBar{ std::make_unique<CommandActionOne>() };
	//std::unique_ptr<Command> InputKey1{ std::make_unique<CommandActionTwo>() };

	//// Gamepad
	//std::unique_ptr<Command> InputDpadUp{ std::make_unique<CommandMoveUp>() };
	//std::unique_ptr<Command> InputDpadLeft{ std::make_unique<CommandMoveLeft>() };
	//std::unique_ptr<Command> InputDpadDown{ std::make_unique<CommandMoveDown>() };
	//std::unique_ptr<Command> InputDpadRight{ std::make_unique<CommandMoveRight>() };

	/* Memory */
	bool isActive_UP{ false };
	bool isActive_DOWN{ false };
	bool isActive_LEFT{ false };
	bool isActive_RIGHT{ false };

	bool isTriggered_ACTION_ONE{ false };
	bool isTriggered_ACTION_TWO{ false };
	bool isTriggered_ACTION_THREE{ false };
	bool isTriggered_ACTION_FOUR{ false };
};

