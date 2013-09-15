
#pragma once

#include <ionCore.h>
#include <ionScene.h>
#include <ionScience.h>


class CSpectrumColorMapper : public IColorMapper
{

public:

	std::string Field;
	double ValueCutoff;
	SRange<f64> AcceptedRange;
	float AlphaIntensity;

	SRange<f64> FieldRange;

	void InitialValues()
	{
		ValueCutoff = 5.0;
		AcceptedRange = SRange<f64>::Full;
		AlphaIntensity = 1.f;
	}

	CSpectrumColorMapper(std::string const inField)
	{
		InitialValues();
		Field = inField;
	}

	virtual SColorAf const GetColor(IDataRecord<f64> const & d)
	{
		f64 const r = d.GetField(Field);
		f32 const v = (f32) FieldRange.Normalize(r);

		f32 Color[4] = {0.f, 0.f, 0.f, 1.f};

		if (v <= 0.0)
		{
		}
		else if (v <= 0.1)
		{
			Color[2] = (f32) ((v - 0.0) / (0.1 - 0.0));
		}
		else if (0.2 >= v && v >= 0.1)
		{
			Color[1] = (f32) ((v - 0.1) / (0.2 - 0.1));
			Color[2] = 1.f;
		}
		else if (0.4 >= v && v >= 0.2)
		{
			Color[1] = 1.f;
			Color[2] = 1.f - (f32) ((v - 0.2) / (0.4 - 0.2));
		}
		else if (0.6 >= v && v >= 0.4)
		{
			Color[0] = (f32) ((v - 0.4) / (0.6 - 0.4));
			Color[1] = 1.f;
		}
		else if (0.8 >= v && v >= 0.6)
		{
			Color[0] = 1.f;
			Color[1] = 1.f - (f32) ((v - 0.6) / (0.8 - 0.6));
		}
		else if (v <= 1.0)
		{
			Color[0] = 1.f;
			Color[1] = Color[2] = (f32) ((v - 0.8) / (1.0 - 0.8));
		}
		else
		{
			Color[0] = Color[1] = Color[2] = 1.f;
		}

		Color[3] = (f32) v;

		return SColorAf(Color[0], Color[1], Color[2], Color[3]);
	}

	virtual void PreProcessValues(IDatabase<f64> & s)
	{
		FieldRange = s.GetFieldRange(Field, ValueCutoff, AcceptedRange);
	}

};
