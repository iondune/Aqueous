#ifndef _SCI_DATA_SET_H_INCLUDED_
#define _SCI_DATA_SET_H_INCLUDED_

#include "SciData.h"
#include "SciDataIterator.h"


class SciDataSet
{

public:

	std::vector<SciData> Values;
	Vector3 DataScale;

	void setDataScale(Vector3 const & v);

	// Clamp field to range
	void normalizeField(std::string const & Field, double const Scale = 1);

	SciDataIterator begin(std::string const & Field);

	SciDataIterator end(std::string const & Field);

};

#endif
