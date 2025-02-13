#pragma once
#include "eDataType.h"
#include "eManagerType.h"
#include "SystemManagerBase.h"

#include <vector>
#include <functional>
#include <utility>
#include <any>

class CentralDispatch
{
public:
	
	/* Connections */
	void connectManager(SystemManagerBase* manager);

	/* Systems Managers Requests */
	std::any receiveManagerPullRequest(const eManagerType& myType, const eDataType& reqDataType, const std::string& reqDataName);
	void forwardManagerPushRequest(const eManagerType& destinationType, const eDataType& pushDataType, std::any pushData);
	std::any searchData(const eDataType& reqDataType, const std::string& reqDataName);

private:
	SystemManagerBase* Commands;
	SystemManagerBase* Controls;
	SystemManagerBase* Data;
	SystemManagerBase* GameState;
	SystemManagerBase* Graphics;
};