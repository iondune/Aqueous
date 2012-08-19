#ifndef _SCI_DATA_SET_H_INCLUDED_
#define _SCI_DATA_SET_H_INCLUDED_

#include <ionTypes.h>

#include "SciData.h"
#include "SciDataIterator.h"


typedef std::pair<double, double> Range;
static Range const FullRange = Range(-std::numeric_limits<double>::max(), std::numeric_limits<double>::max());

bool const inRange(double const v, Range const & r);

class SciDataSet
{

private:

	friend class SciData;
	std::vector<SciData> Values;
	std::map<std::string, std::vector<double> > Fields;
	u32 DataCounter;

public:

	SciDataSet()
		: DataCounter(0)
	{}

	void addData(SciData & Data);

	// Clamp x, y, z to volume range [0,v]
	void setDataScale(Vector3 const & v);

	// Clamp field to range
	void normalizeField(std::string const & Field, double const Scale = 1);

	Range getValueRange(std::string const & Field, double const OutlierCutoff, Range const & acceptedValues = FullRange);

	SciDataIterator begin(std::string const & Field);
	SciDataIterator end(std::string const & Field);

	std::vector<SciData> const & getValues() const;

	u32 const size() const;
	void clear();
	void writeToFile(std::ofstream & File);
	void readFromFile(std::ifstream & File);

	Vector3 DataScale;

};

#endif
