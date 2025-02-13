#pragma once

#include <cstdint>
#include <functional>
#include <vector>
#include <iostream>

/* Used to 
KeyA Global ID for all instanced objects derived from Thing*/
struct IDGenerator
{
public:
	static uint64_t GlobalIDCount;
	static uint64_t getGlobalID();
};

/* The base class for all objects that need to interact with game systems*/

struct Thing
{
public:
	Thing();
	uint64_t GlobalID;
	virtual void run_FrameLogic();
};