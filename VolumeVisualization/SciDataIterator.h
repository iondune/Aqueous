#ifndef _SCI_DATA_ITERATOR_H_INCLUDED_
#define _SCI_DATA_ITERATOR_H_INCLUDED_

#include "SciData.h"


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

template<class Iterator>
Iterator max_element_real(Iterator First, Iterator Last)
{
	Iterator Found = First;
	if (First != Last)
		for (; ++ First != Last; )
			if (*Found != *Found || (*Found < *First && *First == *First))
				Found = First;
	return Found;
}

template<class Iterator>
Iterator min_element_real(Iterator First, Iterator Last)
{
	Iterator Found = First;
	if (First != Last)
		for (; ++ First != Last; )
			if (*Found != *Found || (*First < *Found && *First == *First))
				Found = First;
	return Found;
}

#endif
