#define NDEBUG
/* Project*/ 
#include "CommandManager.h"
#include "ControlsInputPayload.h"
#include "EntityStates.h"
#include "eMovementType.h"
/* STL */
#include <functional>
//#include <memory>
#include <vector>
#include <iostream>
#include <cassert>

CommandManager::CommandManager() {}


void CommandManager::receiveFromCentralDispatch(std::any reqData, const eDataType& reqDataType)
{
	if (reqDataType == eDataType::CONTROLSINPUT)
	{
		FrameInputBuffer.push_back(std::any_cast<ControlsInputPayload>(reqData));
	}
}

eManagerType CommandManager::viewMyManagerType()
{
	return eManagerType(eManagerType::COMMANDS);
}

#pragma warning (disable: 4100)
std::any CommandManager::searchForData(const eDataType& reqDataType, const std::string& reqDataName)
{
	return std::any(0);
}


std::tuple<EntityEvent, EntityEvent> CommandManager::processFrameInput_V2()
{

		//TODO: Game Controller
	for (auto Input : FrameInputBuffer)
	{
		/* Movement Input - Key Releases */
		/* Keyboard Key Up */

		if (Input.Input == UserInput::KeyW_UP)
		{
			isActive_UP = false;
		}
		if (Input.Input == UserInput::KeyA_UP)
		{
			isActive_LEFT = false;
		}
		if (Input.Input == UserInput::KeyS_UP)
		{
			isActive_DOWN = false;
		}
		if (Input.Input == UserInput::KeyD_UP)
		{
			isActive_RIGHT = false;
		}
		if (Input.Input == UserInput::KeySpace_UP)
		{
			isTriggered_ACTION_ONE = false;
		}
		if (Input.Input == UserInput::Key1_R)
		{
			isTriggered_ACTION_TWO = false;
		}

		// Keyboard key down
		if (Input.Input == UserInput::KeyW)
		{
			isActive_UP = true;
		}
		if (Input.Input == UserInput::KeyA)
		{
			isActive_LEFT = true;
		}
		if (Input.Input == UserInput::KeyS)
		{
			isActive_DOWN = true;
		}
		if (Input.Input == UserInput::KeyD)
		{
			isActive_RIGHT = true;
		}
		if (Input.Input == UserInput::KeySpace)
		{
			isTriggered_ACTION_ONE = true;
		}
		if (Input.Input == UserInput::Key1)
		{
			isTriggered_ACTION_TWO = true;
		}

	}

	auto tmp_MovementCommand = resolveMovementInputs();
	auto tmp_ActionCommand = resolveActionInputs();

	/* Empty the buffer to have a clean container for the next frame */
	FrameInputBuffer.clear();

	return std::make_tuple(tmp_MovementCommand, tmp_ActionCommand);
}

EntityEvent CommandManager::resolveMovementInputs()
{
	/* Figure out movement controls */
	if (isActive_UP == true && isActive_LEFT == true)
	{
		return EventMoveUpLeft;
	}
	else if (isActive_UP == true && isActive_RIGHT == true)
	{
		return EventMoveUpRight;
	}
	else if (isActive_DOWN == true && isActive_LEFT == true)
	{
		return EventMoveDownLeft;
	}
	else if (isActive_DOWN == true && isActive_RIGHT == true)
	{
		return EventMoveDownRight;
	}
	else if (isActive_UP == true)
	{
		return EventMoveUp;
	}
	else if (isActive_DOWN == true)
	{
		return EventMoveDown;
	}
	else if (isActive_LEFT == true)
	{
		return EventMoveLeft;
	}
	else if (isActive_RIGHT == true)
	{
		return EventMoveRight;
	}
	else
	{
		return EventIdle;
	}
}

EntityEvent CommandManager::resolveActionInputs()
{
	if (isTriggered_ACTION_ONE == true)
	{
		return EventActionOne;
	}
	else if (isTriggered_ACTION_TWO == true)
	{
		return EventActionTwo;
	}
	else
	{
		return EventActionIdle;
	}
}
