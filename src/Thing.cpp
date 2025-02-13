/* Base class for anything that exists in the world */
#include "Thing.h"

uint64_t IDGenerator::GlobalIDCount{ 0 };

uint64_t IDGenerator::getGlobalID()
{
	++GlobalIDCount;
	return GlobalIDCount;
}

Thing::Thing()
{
	GlobalID = IDGenerator::getGlobalID();
}

void Thing::run_FrameLogic() 
{
}