#ifndef _SCI_DATA_PARSER_H_INCLUDED_
#define _SCI_DATA_PARSER_H_INCLUDED_

#include <vector>
#include <ionScene.h>
#include <ionCore.h>


class SciDataManager;

class SciDataParser
{

public:

	SciDataManager * Manager;

};

class SciDataParserCTD : public SciDataParser
{

public:

	void load(std::string const & FileName);
	void examine(std::string const & FileName);

};

class SciDataParserSimpleTXT : public SciDataParser
{

public:

	void load(std::string const & FileName);

};

class SciDataParserCSV : public SciDataParser
{

public:

	char FieldDelim;
	char ValueDelim;
	bool FieldNames;
	std::vector<std::string> Fields;

	virtual void load(std::string const & FileName);
	virtual void mergedLoad(std::string const & FileName, std::string const & FileName2, std::string const & MatchField);

	SciDataParserCSV()
		: FieldDelim(','), ValueDelim(','), FieldNames(true)
	{}

};

class SciDataParserSmartTether : public SciDataParserCSV
{

public:

	virtual void load(std::string const & FileName);

};

class SciDataParserGrid1 : public SciDataParser
{

public:

	void load(std::string const & FileName);

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

	void load(std::string const & PieFile, std::string const & HoboFile, std::string const & SmartFile, STimeOffsets const & timeOffsets);

};

#endif
