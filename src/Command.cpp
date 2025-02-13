/* Project */
#include "Command.h"
#include "EntityStates.h"

void CommandMoveUp::execute(Entity* ActingEntity)
{
	ActingEntity->add_MovementEvent(EventMoveUp);
}

void CommandMoveDown::execute(Entity* ActingEntity)
{
	ActingEntity->add_MovementEvent(EventMoveDown);
}

void CommandMoveLeft::execute(Entity* ActingEntity)
{
	ActingEntity->add_MovementEvent(EventMoveLeft);
}

void CommandMoveRight::execute(Entity* ActingEntity)
{
	ActingEntity->add_MovementEvent(EventMoveRight);
}

void CommandMoveUpLeft::execute(Entity* ActingEntity)
{
	ActingEntity->add_MovementEvent(EventMoveUpLeft);
}

void CommandMoveUpRight::execute(Entity* ActingEntity)
{
	ActingEntity->add_MovementEvent(EventMoveUpRight);
}

void CommandMoveDownLeft::execute(Entity* ActingEntity)
{
	ActingEntity->add_MovementEvent(EventMoveDownLeft);
}

void CommandMoveDownRight::execute(Entity* ActingEntity)
{
	ActingEntity->add_MovementEvent(EventMoveDownRight);
}

void CommandMoveIdle::execute(Entity* ActingEntity)
{
	ActingEntity->add_MovementEvent(EventIdle);
}

void CommandActionOne::execute(Entity* ActingEntity)
{
	ActingEntity->add_ActionEvent(EventActionOne);
	//std::cout << "COMMAND: Execute Action One\n";
}

void CommandActionTwo::execute(Entity* ActingEntity)
{
	ActingEntity->add_ActionEvent(EventActionTwo);
}

void CommandActionThree::execute(Entity* ActingEntity)
{
	ActingEntity->add_ActionEvent(EventActionThree);
}

void CommandActionFour::execute(Entity* ActingEntity)
{
	ActingEntity->add_ActionEvent(EventActionFour);
}

void CommandActionIdle::execute(Entity* ActingEntity)
{
	ActingEntity->add_ActionEvent(EventIdle);
}
