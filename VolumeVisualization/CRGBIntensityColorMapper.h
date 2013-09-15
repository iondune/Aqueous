
#pragma once

#include <ionCore.h>
#include <ionScene.h>
#include <ionScience.h>


class CRGBIntensityColorMapper : public IColorMapper
{

public:

	std::string Fields[3];
	double ValueCutoff;
	SRange<f64> AcceptedRange;
	float AlphaIntensity;

	SRange<f64> FieldRanges[3];

	void InitialValues()
	{
		ValueCutoff = 5.0;
		AcceptedRange = SRange<f64>();
		AlphaIntensity = 1.f;
	}

	CRGBIntensityColorMapper(std::string const inFields[])
	{
		InitialValues();

		for (int i = 0; i < 3; ++ i)
			Fields[i] = inFields[i];
	}

	CRGBIntensityColorMapper(std::string const & RField, std::string const & GField, std::string const & BField)
	{
		InitialValues();

		Fields[0] = RField;
		Fields[1] = GField;
		Fields[2] = BField;
	}

	virtual SColorAf const GetColor(IDataRecord<f64> const & d)
	{
		float Color[3] = {0.f, 0.f, 0.f};

		for (int i = 0; i < 3; ++ i)
		{
			double const v = d.GetField(Fields[i]);

			Color[i] = (float) ((v - FieldRanges[i].Minimum) / (FieldRanges[i].Maximum - FieldRanges[i].Minimum));
		}

		return SColorAf(Color[0], Color[1], Color[2], Clamp((Color[0] + Color[1] + Color[2]) * AlphaIntensity / 3.f, 0.f, 1.f));
	}

	virtual void PreProcessValues(IDatabase<f64> & s)
	{
		for (int i = 0; i < 3; ++ i)
			FieldRanges[i] = s.GetFieldRange(Fields[i], ValueCutoff, AcceptedRange);
	}

};
