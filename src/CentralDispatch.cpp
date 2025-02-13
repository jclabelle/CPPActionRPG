#pragma warning (disable: 4100)
/* Project */ 
#include "CentralDispatch.h"
/* STL */
#include <iostream>


void CentralDispatch::connectManager(SystemManagerBase* manager)
{
	/* Assign Manager pointers */
	if (manager->viewMyManagerType() == eManagerType::COMMANDS)  Commands = manager;
	if (manager->viewMyManagerType() == eManagerType::CONTROLS)  Controls = manager;
	if (manager->viewMyManagerType() == eManagerType::DATA)		 Data = manager;
	if (manager->viewMyManagerType() == eManagerType::GAMESTATE) GameState = manager;
	if (manager->viewMyManagerType() == eManagerType::GRAPHICS)	 Graphics = manager;

	/* Connect each Manager's inherited functions:
	Manager's PullFromCentralDispatch with CentralDispatch's receiveManagerPullRequest
	Manager's PushToCentralDispatch with CentralDispatch's forwardManagerPushRequest  */
	std::function<std::any(const eManagerType& myType, const eDataType& reqDataType, const std::string& reqDataName)> lambdaManagerPull
		= [&](const eManagerType& myType, const eDataType& reqDataType, const std::string& reqDataName) mutable -> std::any
	{ auto reqData = this->receiveManagerPullRequest(myType, reqDataType, reqDataName);
	return reqData; };
	manager->pullFromCentralDispatch = lambdaManagerPull;

	std::function<void(const eManagerType& destinationType, const eDataType& pushedDataType, std::any pushedData)> lambdaManagerPush
		= [&](const eManagerType& myType, const eDataType& reqDataType, std::any pushedData)  mutable
	{ this->forwardManagerPushRequest(myType, reqDataType, pushedData); };
	manager->pushToCentralDispatch = lambdaManagerPush;
}

/* Fulfill incoming Manager Data requests  */
std::any CentralDispatch::receiveManagerPullRequest(const eManagerType& myType, const eDataType& reqDataType, const std::string& reqDataName)
{
	std::any reqData = searchData(reqDataType, reqDataName);

	if (myType == eManagerType::COMMANDS)
	{
		Commands->receiveFromCentralDispatch(reqData, reqDataType);
		return reqData;
	}

	if (myType == eManagerType::CONTROLS)
	{
		Controls->receiveFromCentralDispatch(reqData, reqDataType);
		return reqData;
	}

	if (myType == eManagerType::DATA)
	{
		Data->receiveFromCentralDispatch(reqData, reqDataType);
		return reqData;
	}

	if (myType == eManagerType::GAMESTATE)
	{
		GameState->receiveFromCentralDispatch(reqData, reqDataType);
		return reqData;
	}

	if (myType == eManagerType::GRAPHICS)
	{
		Graphics->receiveFromCentralDispatch(reqData, reqDataType);
		return reqData;
	}
	return reqData;
}

/* Forward Data from a sending Manager to a Receiving Manager */
void CentralDispatch::forwardManagerPushRequest(const eManagerType& destinationType, const eDataType& pushedDataType, std::any pushedData)
{
	if (destinationType == eManagerType::COMMANDS)
	{
		Commands->receiveFromCentralDispatch(pushedData, pushedDataType);
	}

	if (destinationType == eManagerType::CONTROLS)
	{
		Controls->receiveFromCentralDispatch(pushedData, pushedDataType);
	}

	if (destinationType == eManagerType::DATA)
	{
		Data->receiveFromCentralDispatch(pushedData, pushedDataType);
	}

	if (destinationType == eManagerType::GAMESTATE)
	{
		GameState->receiveFromCentralDispatch(pushedData, pushedDataType);
	}

	if (destinationType == eManagerType::GRAPHICS)
	{
		Graphics->receiveFromCentralDispatch(pushedData,pushedDataType);
	}
}

/* Search a Manager's WorldData for a specific item using it's DataType and Name */
std::any CentralDispatch::searchData(const eDataType& reqDataType, const std::string& reqDataName)
{
	std::any returnData;

	returnData = Data->searchForData(reqDataType, reqDataName);

	return returnData;

}
