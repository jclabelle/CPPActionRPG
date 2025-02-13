#pragma once
#include "Entity.h"

/* Entity Commands used by the Command Manager */
class Command
{
public:
	virtual ~Command() {}
	virtual void execute(Entity* ActingEntity) = 0;
};

class CommandMoveUp : public Command
{
public:
	virtual void execute(Entity* ActingEntity) override;
};

class CommandMoveDown : public Command
{
public:
	virtual void execute(Entity* ActingEntity) override;
};

class CommandMoveLeft : public Command
{
public:
	virtual void execute(Entity* ActingEntity) override;
};

class CommandMoveRight : public Command
{
public:
	virtual void execute(Entity* ActingEntity) override;
};

class CommandMoveUpLeft : public Command
{
public:
	virtual void execute(Entity* ActingEntity) override;
};

class CommandMoveUpRight : public Command
{
public:
	virtual void execute(Entity* ActingEntity) override;
};

class CommandMoveDownLeft : public Command
{
public:
	virtual void execute(Entity* ActingEntity) override;
};

class CommandMoveDownRight : public Command
{
public:
	virtual void execute(Entity* ActingEntity) override;
};

class CommandMoveIdle : public Command
{
public:
	virtual void execute(Entity* ActingEntity) override;
};

class CommandActionOne : public Command
{
public:
	virtual void execute(Entity* ActingEntity) override;
};

class CommandActionTwo : public Command
{
public:
	virtual void execute(Entity* ActingEntity) override;
};

class CommandActionThree : public Command
{
public:
	virtual void execute(Entity* ActingEntity) override;
};

class CommandActionFour : public Command
{
public:
	virtual void execute(Entity* ActingEntity) override;
};

class CommandActionIdle :public Command
{
public:
	virtual void execute(Entity* ActingEntity) override;
};
