#ifndef _SCI_DATA_SET_H_INCLUDED_
#define _SCI_DATA_SET_H_INCLUDED_

#include "SciData.h"
#include "SciDataIterator.h"


class SciDataSet
{

public:

	std::vector<SciData> Values;
	Vector3 DataScale;
	
	// Clamp x, y, z to volume range [0,v]
	void setDataScale(Vector3 const & v);

	// Clamp field to range
	void normalizeField(std::string const & Field, double const Scale = 1);

	std::pair<double, double> getValueRange(std::string const & Field, double const OutlierCutoff);

	SciDataIterator begin(std::string const & Field);
	SciDataIterator end(std::string const & Field);

};

#endif
