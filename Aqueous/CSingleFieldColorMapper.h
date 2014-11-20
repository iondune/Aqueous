
#pragma once

#include <ionCore.h>
#include <ionScene.h>
#include <ionScience.h>


class CSingleFieldColorMapper : public IColorMapper
{

public:

	std::string Field;
	double ValueCutoff;
	SRange<f64> AcceptedRange;
	float AlphaIntensity;

	SRange<f64> FieldRange;

	void initialValues()
	{
		ValueCutoff = 5.0;
		AcceptedRange = SRange<f64>();
		AlphaIntensity = 1.f;
	}

	CSingleFieldColorMapper(std::string const inField)
	{
		initialValues();

		Field = inField;
	}

	virtual SColorAf const GetColor(IDataRecord<f64> const & d)
	{
		f32 Color[3] = {0.f, 0.f, 0.f};

		f64 const v = d.GetField(Field);
		f32 const r = (f32) FieldRange.Normalize(v);

		Color[0] = Color[2] = 1.f - r;
		Color[1] = r;
		

		return SColorAf(Color[0], Color[1], Color[2], r);
	}

	virtual void PreProcessValues(IDatabase<f64> & s)
	{
		FieldRange = s.GetFieldRange(Field, ValueCutoff, AcceptedRange);
	}

};
