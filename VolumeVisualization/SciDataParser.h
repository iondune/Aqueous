
#pragma once

#include <ionCore.h>
#include <ionScene.h>


f64 const string_to_double(std::string const & s);

class CDataSet;

class SciDataParser
{

public:

	CDataSet * DataSet;
	string FileName;
	string Field;

	virtual void Load() = 0;

};

class SciDataParserCTD : public SciDataParser
{

public:

	void Load();
	void examine(std::string const & FileName);

};

class SciDataParserSimpleTXT : public SciDataParser
{

public:

	void Load();

};

class SciDataParserCSV : public SciDataParser
{

public:

	char FieldDelim;
	char ValueDelim;
	bool FieldNames;
	std::vector<std::string> Fields;
	
	void Load();

	SciDataParserCSV()
		: FieldDelim(','), ValueDelim(','), FieldNames(true)
	{}

};

class SciDataParserMergedCSV : public SciDataParser
{

public:

	char FieldDelim;
	char ValueDelim;
	bool FieldNames;
	string OtherFileName;
	string MatchField;
	std::vector<std::string> Fields;
	
	void Load();

	SciDataParserMergedCSV()
		: FieldDelim(','), ValueDelim(','), FieldNames(true)
	{}

};

class SciDataParserSmartTether : public SciDataParserCSV
{

public:
	
	void Load();

};

class SciDataParserGrid1 : public SciDataParser
{

public:
	
	void Load();

};

enum class ETimes
{
	Pie = 0,
	Hobo = 1,
	SmartTether = 2
};

struct STimeOffsets
{
	int * timeOffsets;

	STimeOffsets(int pieOffset, int hoboOffset, int tetherOffset)
	{
		timeOffsets = new int[3];
		timeOffsets[0] = pieOffset;
		timeOffsets[1] = hoboOffset;
		timeOffsets[2] = tetherOffset;
	}

	int const operator[] (ETimes const index) const
	{
		return timeOffsets[(int) index];
	}
};

class SciDataParserPieSlices : public SciDataParser
{

public:

	string HoboFile;
	string SmartFile;
	STimeOffsets TimeOffsets;
	
	void Load();

};
