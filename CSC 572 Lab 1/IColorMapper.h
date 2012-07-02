#ifndef _ICOLORMAPPER_H_INCLUDED_
#define _ICOLORMAPPER_H_INCLUDED_

#include <ionScene.h>
#include <ionCore.h>

#include "SciDataSet.h"
#include "SciDataTree.h"

class IColorMapper
{

public:

	virtual SColor const getColor(SciData const & d) = 0;
	virtual void preProcessValues(SciDataSet & s) = 0;

};

#endif
