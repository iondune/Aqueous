#include "SciData.h"

#include "SciDataSet.h"


SciData::SciData()
	: InternalIndex(-1), ContainingSet(0)
{}

SciData::SciData(double x, double y, double z)
	: Location(x, y, z), InternalIndex(-1), ContainingSet(0)
{}

SciData::~SciData()
{}

Vector3 const & SciData::getLocation() const
{
	return Location;
}

double const SciData::getField(std::string const & Field) const
{
	if (Field == "x")
		return Location.X;
	if (Field == "y")
		return Location.Y;
	if (Field == "z")
		return Location.Z;

	std::map<std::string, std::vector<double> >::const_iterator it;
	if ((it = ContainingSet->Fields.find(Field)) == ContainingSet->Fields.end())
		return 0;
	
	return it->second[InternalIndex];
}

double & SciData::getField(std::string const & Field)
{
	static double Dummy = 0;

	if (Field == "x")
		return Location.X;
	if (Field == "y")
		return Location.Y;
	if (Field == "z")
		return Location.Z;

	std::map<std::string, std::vector<double> >::iterator it;
	if ((it = ContainingSet->Fields.find(Field)) == ContainingSet->Fields.end())
		return Dummy;
	
	return it->second[InternalIndex];
}

double & SciData::addField(std::string const & Field)
{
	if (Field == "x")
		return Location.X;
	if (Field == "y")
		return Location.Y;
	if (Field == "z")
		return Location.Z;

	std::map<std::string, std::vector<double> >::iterator it;
	if ((it = ContainingSet->Fields.find(Field)) == ContainingSet->Fields.end())
	{
		ContainingSet->Fields[Field].resize(ContainingSet->DataCounter, 0.0);
		return ContainingSet->Fields[Field][InternalIndex];
	}
	
	return it->second[InternalIndex];
}
