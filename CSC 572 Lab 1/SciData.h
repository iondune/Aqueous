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
		std::map<std::string, double>::const_iterator it;
		if ((it = ScalarFields.find(Field)) == ScalarFields.end())
			return 0;
		else
			return it->second;
	}

};

class SciDataIterator
{

	std::vector<SciData>::iterator Iterator;
	std::string const & Field;

public:

	SciDataIterator(std::vector<SciData>::iterator iterator, std::string const & field)
		: Iterator(iterator), Field(field)
	{}

	SciDataIterator & operator ++()
	{
		++ Iterator;
		return * this;
	}

	SciDataIterator operator ++(int)
	{
		SciDataIterator temp = * this;
		++ Iterator;
		return temp;
	}

	double const operator *() const
	{
		std::map<std::string, double>::const_iterator it;
		if ((it = Iterator->ScalarFields.find(Field)) == Iterator->ScalarFields.end())
			return 0;
		else
			return it->second;
	}

	double & operator *()
	{
		static double dummy = 0;
		std::map<std::string, double>::iterator it;
		if ((it = Iterator->ScalarFields.find(Field)) == Iterator->ScalarFields.end())
			return dummy;
		else
			return it->second;
	}

	double const * const operator ->() const
	{
		std::map<std::string, double>::const_iterator it;
		if ((it = Iterator->ScalarFields.find(Field)) == Iterator->ScalarFields.end())
			return 0;
		else
			return & it->second;
	}

	double * const operator ->()
	{
		std::map<std::string, double>::iterator it;
		if ((it = Iterator->ScalarFields.find(Field)) == Iterator->ScalarFields.end())
			return 0;
		else
			return & it->second;
	}

	bool const operator < (SciDataIterator const & other) const
	{
		return ** this < * other;
	}

	bool const operator != (SciDataIterator const & other) const
	{
		return ! equals(** this, * other);
	}

};

class SciDataSet
{

public:

	std::vector<SciData> Values;

	void normalizeField(std::string const & Field)
	{
		double max = * std::max(begin(Field), end(Field)), min = * std::min(begin(Field), end(Field));
		std::for_each(begin(Field), end(Field), [min, max](double & d) { d = (d - min) / (max - min); });
	}

	SciDataIterator begin(std::string const & Field)
	{
		return SciDataIterator(Values.begin(), Field);
	}

	SciDataIterator end(std::string const & Field)
	{
		return SciDataIterator(Values.end(), Field);
	}
};

#include "ionCore.h"
#include <vector>

class ISciTreeNode
{

public:

	SBoundingBox3 Extents;

	virtual bool const isLeaf() const =0;

	virtual ~ISciTreeNode()
	{}

};

enum EOctant
{
	EO_TOP_UPPER_LEFT,
	EO_TOP_UPPER_RIGHT,
	EO_TOP_LOWER_LEFT,
	EO_TOP_LOWER_RIGHT,
	EO_BOTTOM_UPPER_LEFT,
	EO_BOTTOM_UPPER_RIGHT,
	EO_BOTTOM_LOWER_LEFT,
	EO_BOTTOM_LOWER_RIGHT,
	EO_COUNT
};

class CSciTreeNode : public ISciTreeNode
{

public:

	ISciTreeNode * Children[EO_COUNT];

	virtual bool const isLeaf() const
	{
		return false;
	}

};

class CSciTreeLeaf : public ISciTreeNode
{

public:

	std::vector<SciData> Datums;

	virtual bool const isLeaf() const
	{
		return true;
	}

};

#endif
