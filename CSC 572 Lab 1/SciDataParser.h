#ifndef _SCI_DATA_PARSER_H_INCLUDED_
#define _SCI_DATA_PARSER_H_INCLUDED_

#include "SciDataSet.h"

#include <vector>

class SciDataParser : public SciDataSet
{

public:

	int parseTXTFile(std::string const & data);
	int parseMATFile(std::string const & data);
	int parseMATGridFile(std::string const & data);

	unsigned int VolumeHandle;

};

#endif
