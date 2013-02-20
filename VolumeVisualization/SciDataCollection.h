#ifndef _SCI_DATA_COLLECTION_H_INCLUDED_
#define _SCI_DATA_COLLECTION_H_INCLUDED_

#include <ionTypes.h>

#include "SciData.h"
#include "SciDataIterator.h"
#include "Range.h"


class SciDataCollection
{

private:

	friend class SciData;
	std::vector<SciData> Values;
	std::map<std::string, std::vector<double> > Fields;
	u32 DataCounter;

public:

	SciDataCollection()
		: DataCounter(0)
	{}
	
	void addData(SciData & Data);

	//! Assumes internalIndex is valid, used for file IO
	void addData(SciData & Data, s32 const internalIndex);

	//! Clamp x, y, z to volume range [0,v]
	void rescaleData(vec3d const & v);

	vec3d const getDataScale();

	//! Clamp field to range
	void normalizeField(std::string const & Field, double const Scale = 1);

	//! \param OutlierCutoff Values outside of this many standard deviations are excluded
	Range getValueRange(std::string const & Field, double const OutlierCutoff = 5, Range const & acceptedValues = FullRange);

	SciDataIterator begin(std::string const & Field);
	SciDataIterator end(std::string const & Field);

	std::vector<SciData> const & getValues() const;

	u32 const size() const;
	void clear();
	void writeToFile(std::ofstream & File);
	void readFromFile(std::ifstream & File);

	struct STraits
	{
		std::string PositionXField, PositionYField, PositionZField;

		STraits()
			: PositionXField("x"), PositionYField("y"), PositionZField("z")
		{}
	};

	STraits Traits;

};

#endif
