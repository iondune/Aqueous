#ifndef _SCI_DATA_H_INCLUDED_
#define _SCI_DATA_H_INCLUDED_

#include <string>
#include <algorithm>
#include <map>
#include <vector>

#include <SVector3.h>
typedef SVector3d Vector3;


class SciData
{

public:

	SciData()
	{}

	SciData(double x, double y, double z)
		: Location(x, y, z)
	{}

	~SciData()
	{}

	Vector3 Location;
	std::map<std::string, double> ScalarFields;

	Vector3 const & getLocation() const
	{
		return Location;
	}

	double const getField(std::string const & Field) const
	{
		if (Field == "x")
			return Location.X;
		if (Field == "y")
			return Location.Y;
		if (Field == "z")
			return Location.Z;

		std::map<std::string, double>::const_iterator it;
		if ((it = ScalarFields.find(Field)) == ScalarFields.end())
			return 0;
		else
			return it->second;
	}

	double & getField(std::string const & Field)
	{
		static double dummy = 0;

		if (Field == "x")
			return Location.X;
		if (Field == "y")
			return Location.Y;
		if (Field == "z")
			return Location.Z;

		std::map<std::string, double>::iterator it;
		if ((it = ScalarFields.find(Field)) == ScalarFields.end())
			return dummy;
		else
			return it->second;
	}

};

#endif
