
#pragma once

#include "CProgramContext.h"


class ILocation
{

public:
	
	longlatd LowerBound, UpperBound;
	
	virtual void Load() = 0;
	virtual void ConcurrentLoad() = 0;
	virtual void InitSceneElements(CProgramContext::SScene & Scene) = 0;

};
