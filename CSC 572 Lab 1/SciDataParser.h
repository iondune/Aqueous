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
