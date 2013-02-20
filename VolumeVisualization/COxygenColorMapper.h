#ifndef _COXYGENCOLORMAPPER_H_INCLUDED_
#define _COXYGENCOLORMAPPER_H_INCLUDED_

#include <ionScene.h>
#include <ionCore.h>

#include "SciDataCollection.h"
#include "IColorMapper.h"


class COxygenColorMapper : public IColorMapper
{

public:

	std::string Field;

	COxygenColorMapper(std::string const & field = "o1")
		: Field(field)
	{}

	virtual SColorAf const getColor(SciData const & d)
	{
		double const v = d.getField(Field);

		float Color[4] = {0.f, 0.f, 0.f, 0.f};

		if (30500.0 >= v && v >= 29000.0)
		{
			Color[2] = (float) ((v - 29000.0) / (30500.0 - 29000.0));
		}
		else if (31800.0 >= v && v >= 30500.0)
		{
			Color[1] = (float) ((v - 30500.0) / (31800.0 - 30500.0));
			Color[2] = 1.f;
		}
		else if (32500.0 >= v && v >= 31800.0)
		{
			Color[1] = 1.f;
			Color[2] = 1.f - (float) ((v - 31800.0) / (32500.0 - 31800.0));
		}
		else if (33200.0 >= v && v >= 32500.0)
		{
			Color[0] = (float) ((v - 32500.0) / (33200.0 - 32500.0));
			Color[1] = 1.f;
		}
		else if (34200.0 >= v && v >= 33200.0)
		{
			Color[0] = 1.f;
			Color[1] = 1.f - (float) ((v - 33200.0) / (34200.0 - 33200.0));
		}
		else if (36000.0 >= v && v >= 34200.0)
		{
			Color[0] = 1.f - (float) ((v - 34200.0) / (36000.0 - 34200.0));
		}

		Color[3] = (float) ((v - 29000.0) / (36000.0 - 29000.0));

		return SColorAf(Color[0], Color[1], Color[2], Color[3]);
	}

	virtual void preProcessValues(SciDataCollection & s)
	{
	}

};

#endif
