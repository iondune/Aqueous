#ifndef _ICOLORMAPPER_H_INCLUDED_
#define _ICOLORMAPPER_H_INCLUDED_

#include <ionScene.h>
#include <ionCore.h>

#include "SciDataCollection.h"


class IColorMapper
{

public:

	virtual SColorAf const getColor(SciData const & d) = 0;
	virtual void preProcessValues(SciDataCollection & s) = 0;

};

#endif
