#pragma once
/* Projet */
#include "eManagerType.h"
#include "eDataType.h"
#include "WorldData.h"
/* STL */
#include <string>
#include <any>


struct SystemManagerBase
{

	/* Ask */
	std::function<std::any(const eManagerType& myType, const eDataType& reqDataType, const std::string& reqDataName)> pullFromCentralDispatch;
	std::function<void(const eManagerType& destinationType, const eDataType& pushedDataType, std::any pushedData)> pushToCentralDispatch;

	/* Receive */
	virtual void receiveFromCentralDispatch(std::any reqData, const eDataType& reqDataType) =0;
	virtual std::any searchForData(const eDataType& reqDataType, const std::string& reqDataName) = 0;
	virtual eManagerType viewMyManagerType()=0;

};