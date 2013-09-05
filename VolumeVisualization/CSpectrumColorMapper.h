
#pragma once

#include <ionScene.h>
#include <ionCore.h>

#include "IColorMapper.h"


class CSpectrumColorMapper : public IColorMapper
{

public:

	std::string Field;
	double ValueCutoff;
	Range AcceptedRange;
	float AlphaIntensity;

	Range FieldRange;

	void InitialValues()
	{
		ValueCutoff = 5.0;
		AcceptedRange = FullRange;
		AlphaIntensity = 1.f;
	}

	CSpectrumColorMapper(std::string const inField)
	{
		InitialValues();
		Field = inField;
	}

	virtual SColorAf const GetColor(STable::SRow const & d)
	{
		double const r = d.GetField(Field);
		float const v = (float) ((r - FieldRange.first) / (FieldRange.second - FieldRange.first));

		float Color[4] = {0.f, 0.f, 0.f, 1.f};

		if (v <= 0.0)
		{
		}
		else if (v <= 0.1)
		{
			Color[2] = (float) ((v - 0.0) / (0.1 - 0.0));
		}
		else if (0.2 >= v && v >= 0.1)
		{
			Color[1] = (float) ((v - 0.1) / (0.2 - 0.1));
			Color[2] = 1.f;
		}
		else if (0.4 >= v && v >= 0.2)
		{
			Color[1] = 1.f;
			Color[2] = 1.f - (float) ((v - 0.2) / (0.4 - 0.2));
		}
		else if (0.6 >= v && v >= 0.4)
		{
			Color[0] = (float) ((v - 0.4) / (0.6 - 0.4));
			Color[1] = 1.f;
		}
		else if (0.8 >= v && v >= 0.6)
		{
			Color[0] = 1.f;
			Color[1] = 1.f - (float) ((v - 0.6) / (0.8 - 0.6));
		}
		else if (v <= 1.0)
		{
			Color[0] = 1.f;
			Color[1] = Color[2] = (float) ((v - 0.8) / (1.0 - 0.8));
		}
		else
		{
			Color[0] = Color[1] = Color[2] = 1.f;
		}

		Color[3] = (float) v;

		return SColorAf(Color[0], Color[1], Color[2], Color[3]);
	}

	virtual void PreProcessValues(STable & s)
	{
		FieldRange = s.GetFieldRange(Field, ValueCutoff, AcceptedRange);
	}

};
