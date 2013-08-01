#ifndef _SCI_DATA_H_INCLUDED_
#define _SCI_DATA_H_INCLUDED_

#include <string>
#include <algorithm>
#include <map>
#include <vector>

#include <ionTypes.h>
#include <ionMath.h>


class SciData
{

private:

	friend class SciDataCollection;

	s32 InternalIndex;
	SciDataCollection * ContainingSet;

public:
	
	SciData(SciDataCollection & containingSet);
	SciData(SciDataCollection & containingSet, s32 const internalIndex);
	~SciData();

	double const getField(std::string const & Field) const;
	double & getField(std::string const & Field);
	double & addField(std::string const & Field);

	vec3d const getPosition() const;

};

#endif
