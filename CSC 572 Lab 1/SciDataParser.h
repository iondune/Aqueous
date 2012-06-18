#include "SciData.h"

#include <vector>

class SciDataParser
{

public:
	
	int parseFile(std::string const &data);

	SVector3 m_minLoc;// = Vector3(DBL_MAX,DBL_MAX,DBL_MAX);
	SVector3 m_maxLoc;// = Vector3(-DBL_MAX,-DBL_MAX,-DBL_MAX);
	double m_minO2;// = DBL_MAX;
	double m_maxO2;// = -DBL_MAX;

	double mind1;// = DBL_MAX;
   double maxd1;// = -DBL_MAX;

	std::vector<SciData> m_data;
};
