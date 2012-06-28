#ifndef _SCI_DATA_PARSER_H_INCLUDED_
#define _SCI_DATA_PARSER_H_INCLUDED_

#include <vector>
#include <ionScene.h>

#include "SciDataSet.h"
#include "SciDataTree.h"

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

	void generateVolumeFromGridValues(std::string const & RField, std::string const & GField, std::string const & BField, double const StandardDeviations = 5.0);
	void createDataTreeFromRawValues();

};

class SciDataParserCTD : public SciDataParser
{

public:

	SciDataParserCTD(std::string const & FileName);

};

class SciDataParserSimpleTXT : public SciDataParser
{

public:

	SciDataParserSimpleTXT(std::string const & FileName);

};

class SciDataParserGrid1 : public SciDataParser
{

public:

	SciDataParserGrid1(std::string const & FileName);

};

#endif
