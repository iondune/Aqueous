#ifndef _CSINGLEFIELDCOLORMAPPER_H_INCLUDED_
#define _CSINGLEFIELDCOLORMAPPER_H_INCLUDED_

#include <ionScene.h>
#include <ionCore.h>

#include "SciDataCollection.h"
#include "IColorMapper.h"


class CSingleFieldColorMapper : public IColorMapper
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
		AcceptedRange = Range();
		AlphaIntensity = 1.f;
	}

	CSingleFieldColorMapper(std::string const inField)
	{
		initialValues();

		Field = inField;
	}

	virtual SColorAf const getColor(SciData const & d)
	{
		float Color[3] = {0.f, 0.f, 0.f};

		double const v = d.getField(Field);
		float const r = (float) ((v - FieldRange.first) / (FieldRange.second - FieldRange.first));

		Color[0] = Color[2] = 1.f - r;
		Color[1] = r;
		

		return SColorAf(Color[0], Color[1], Color[2], r);
	}

	virtual void preProcessValues(SciDataCollection & s)
	{
		FieldRange = s.getValueRange(Field, ValueCutoff, AcceptedRange);
	}

};

#endif
