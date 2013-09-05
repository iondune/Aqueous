
#pragma once

#include <ionScene.h>
#include <ionCore.h>

#include "STable.h"


class IColorMapper
{

public:

	virtual SColorAf const GetColor(STable::SRow const & d) = 0;
	virtual void PreProcessValues(STable & s) = 0;

};
