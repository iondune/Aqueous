#ifndef _SCI_DATA_PARSER_H_INCLUDED_
#define _SCI_DATA_PARSER_H_INCLUDED_

#include <vector>
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

class CRGBIntensityColorMapper : public IColorMapper
{

public:

	std::string Fields[3];
	double ValueCutoff;
	Range AcceptedRange;
	float AlphaIntensity;

	Range FieldRanges[3];

	void initialValues()
	{
		ValueCutoff = 5.0;
		AcceptedRange = Range();
		AlphaIntensity = 1.f;
	}

	CRGBIntensityColorMapper(std::string const inFields[])
	{
		initialValues();

		for (int i = 0; i < 3; ++ i)
			Fields[i] = inFields[i];
	}

	CRGBIntensityColorMapper(std::string const & RField, std::string const & GField, std::string const & BField)
	{
		initialValues();

		Fields[0] = RField;
		Fields[1] = GField;
		Fields[2] = BField;
	}

	virtual SColor const getColor(SciData const & d)
	{
		float Color[3] = {0.f, 0.f, 0.f};

		for (int i = 0; i < 3; ++ i)
		{
			double const v = d.getField(Fields[i]);

			Color[i] = (float) ((v - FieldRanges[i].first) / (FieldRanges[i].second - FieldRanges[i].first));
		}

		return SColor(Color[0], Color[1], Color[2], clamp((Color[0] + Color[1] + Color[2]) * AlphaIntensity / 3.f, 0.f, 255.f));
	}

	virtual void preProcessValues(SciDataSet & s)
	{
		for (int i = 0; i < 3; ++ i)
			FieldRanges[i] = s.getValueRange(Fields[i], ValueCutoff, AcceptedRange);
	}

};



class COxygenColorMapper : public IColorMapper
{

public:

	COxygenColorMapper()
	{}

	virtual SColor const getColor(SciData const & d)
	{
		double const v = d.getField("o1");

		float Color[3] = {0.f, 0.f, 0.f};

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

		return SColor(Color[0], Color[1], Color[2], 0.8f);
	}

	virtual void preProcessValues(SciDataSet & s)
	{
	}

};


class COxygenLocalizedColorMapper : public COxygenColorMapper
{

public:

	float EmphasisLocation;
	Range HeightRange;

	float LocalRange;
	float MinimumAlpha;

	COxygenLocalizedColorMapper()
		: EmphasisLocation(0.5f)
	{
		LocalRange = 0.1f;
		MinimumAlpha = 0.03f;
	}

	virtual SColor const getColor(SciData const & d)
	{
		SColor c = COxygenColorMapper::getColor(d);

		double Z = d.getField("z");
		float Height = (float) ((Z - HeightRange.first) / (HeightRange.second - HeightRange.first));

		if (abs(Height - EmphasisLocation) < LocalRange / 2.f)
		{
			float Ratio = 1.f - abs(Height - EmphasisLocation) / (LocalRange / 2.f);
			c.Alpha = Ratio * (1.f - MinimumAlpha) + MinimumAlpha;
		}
		else
		{
			c.Alpha = MinimumAlpha;
		}

		return c;
	}

	virtual void preProcessValues(SciDataSet & s)
	{
		HeightRange = s.getValueRange("z", 5.0);
	}

};

class SciDataParser
{

public:

	SciDataParser()
		: VolumeHandle(0)
	{}

	SciDataSet RawValues;
	SciDataSet GridValues;

	unsigned int VolumeHandle;
	ISciTreeNode * DataTree;

	int const * GridDimensions;

	void generateVolumeFromGridValues(IColorMapper * ColorMapper);
	void createDataTreeFromRawValues();

	virtual void load(std::string const & FileName) =0;

};

class SciDataParserCTD : public SciDataParser
{

public:

	void load(std::string const & FileName);

};

class SciDataParserSimpleTXT : public SciDataParser
{

public:

	void load(std::string const & FileName);

};

class SciDataParserGrid1 : public SciDataParser
{

public:

	void load(std::string const & FileName);

};

#endif
