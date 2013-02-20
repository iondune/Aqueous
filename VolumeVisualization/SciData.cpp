#include "SciData.h"

#include "SciDataCollection.h"


SciData::SciData(SciDataCollection & containingSet)
	: InternalIndex(-1), ContainingSet(&containingSet)
{
	ContainingSet->addData(*this);
}

SciData::SciData(SciDataCollection & containingSet, s32 const internalIndex)
	: InternalIndex(internalIndex), ContainingSet(&containingSet)
{
	ContainingSet->addData(*this, InternalIndex);
}

SciData::~SciData()
{}


double const SciData::getField(std::string const & Field) const
{
	std::map<std::string, std::vector<double> >::const_iterator it;
	if ((it = ContainingSet->Fields.find(Field)) == ContainingSet->Fields.end())
		return 0;
	
	return it->second[InternalIndex];
}

double & SciData::getField(std::string const & Field)
{
	static double Dummy = 0;

	std::map<std::string, std::vector<double> >::iterator it;
	if ((it = ContainingSet->Fields.find(Field)) == ContainingSet->Fields.end())
		return Dummy;
	
	return it->second[InternalIndex];
}

double & SciData::addField(std::string const & Field)
{
	std::map<std::string, std::vector<double> >::iterator it;
	if ((it = ContainingSet->Fields.find(Field)) == ContainingSet->Fields.end())
	{
		ContainingSet->Fields[Field].resize(ContainingSet->DataCounter, 0.0);
		return ContainingSet->Fields[Field][InternalIndex];
	}
	
	return it->second[InternalIndex];
}

vec3d const SciData::getPosition() const
{
	return vec3d(
		getField(ContainingSet->Traits.PositionXField),
		getField(ContainingSet->Traits.PositionYField),
		getField(ContainingSet->Traits.PositionZField));
}
