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

class SciDataIterator
{

	std::vector<SciData>::iterator Iterator;
	std::string Field;

public:

	typedef std::vector<SciData>::iterator::iterator_category iterator_category;
	typedef double value_type;
	typedef std::vector<SciData>::iterator::difference_type difference_type;
	typedef double * pointer;
	typedef double & reference;

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
		return Iterator->getField(Field);
	}

	double & operator *()
	{
		return Iterator->getField(Field);
	}

	double const * const operator ->() const
	{
		return & Iterator->getField(Field);
	}

	double * const operator ->()
	{
		return & Iterator->getField(Field);
	}

	bool const operator < (SciDataIterator const & other) const
	{
		return Iterator < other.Iterator;
	}

	bool const operator != (SciDataIterator const & other) const
	{
		return Iterator != other.Iterator;
	}

};

template<class _FwdIt>
	_FwdIt max_element_real(_FwdIt _First, _FwdIt _Last)
	{	// find largest element, using operator<
	_FwdIt _Found = _First;
	if (_First != _Last)
		for (; ++_First != _Last; )
			if (*_Found != *_Found || (_DEBUG_LT(*_Found, *_First) && *_First == *_First))
				_Found = _First;
	return (_Found);
	}

template<class _FwdIt>
	_FwdIt min_element_real(_FwdIt _First, _FwdIt _Last)
	{	// find smallest element, using operator<
	_FwdIt _Found = _First;
	if (_First != _Last)
		for (; ++_First != _Last; )
			if (*_Found != *_Found || (_DEBUG_LT(*_First, *_Found) && *_First == *_First))
				_Found = _First;
	return (_Found);
	}

class SciDataSet
{

public:

	std::vector<SciData> Values;
	Vector3 DataScale;

	void setDataScale(Vector3 const & v)
	{
		normalizeField("x", v.X);
		normalizeField("y", v.Y);
		normalizeField("z", v.Z);

		DataScale = v;
	}

	void normalizeField(std::string const & Field, double const Scale = 1)
	{
		double max = * max_element_real(begin(Field), end(Field)), min = * min_element_real(begin(Field), end(Field));
		std::for_each(begin(Field), end(Field), [min, max, Scale](double & d) { d = (d - min) / (max - min) * Scale; });
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
