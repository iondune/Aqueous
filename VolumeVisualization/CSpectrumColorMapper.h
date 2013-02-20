#ifndef _CSPECTRUMCOLORMAPPER_H_INCLUDED_
#define _CSPECTRUMCOLORMAPPER_H_INCLUDED_

#include <ionScene.h>
#include <ionCore.h>

#include "SciDataCollection.h"
#include "IColorMapper.h"


class CSpectrumColorMapper : public IColorMapper
{

public:

	std::string Field;
	double ValueCutoff;
	Range AcceptedRange;
	float AlphaIntensity;

	Range FieldRange;

	void initialValues()
	{
		ValueCutoff = 5.0;
		AcceptedRange = FullRange;
		AlphaIntensity = 1.f;
	}

	CSpectrumColorMapper(std::string const inField)
	{
		initialValues();
		Field = inField;
	}

	virtual SColorAf const getColor(SciData const & d)
	{
		double const r = d.getField(Field);
		float const v = (float) ((r - FieldRange.first) / (FieldRange.second - FieldRange.first));

		float Color[4] = {0.f, 0.f, 0.f, 0.f};

		if (v <= 0.1)
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
		else if (v >= 0.8)
		{
			Color[0] = 1.f;
			Color[1] = Color[2] = (float) ((v - 0.8) / (1.0 - 0.8));
		}

		Color[3] = (float) v;

		return SColorAf(Color[0], Color[1], Color[2], Color[3]);
	}

	virtual void preProcessValues(SciDataCollection & s)
	{
		FieldRange = s.getValueRange(Field, ValueCutoff, AcceptedRange);
	}

};

#endif
