#include "SciData.h"

#include <vector>

class SciDataParser : public SciDataSet
{

public:

	int parseTXTFile(std::string const & data);
	int parseMATFile(std::string const & data);
	int parseMATGridFile(std::string const & data);

};
