#ifndef _SCI_DATA_H_INCLUDED_
#define _SCI_DATA_H_INCLUDED_

#include <string>
#include <algorithm>
#include <map>
#include <vector>

#include <ionTypes.h>
#include <SVector3.h>
typedef SVector3d Vector3;


class SciDataSet;

class SciData
{

private:

	friend class SciDataSet;

	s32 InternalIndex;
	SciDataSet * ContainingSet;

public:

	SciData();
	SciData(double x, double y, double z);
	~SciData();

	Vector3 Location;

	Vector3 const & getLocation() const;
	double const getField(std::string const & Field) const;
	double & getField(std::string const & Field);
	double & addField(std::string const & Field);

};

#endif
