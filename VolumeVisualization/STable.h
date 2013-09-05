
#pragma once

#include <ionCore.h>
#include <ionMath.h>

#include "Range.h"


class STable
{

public:

	class SRow
	{

		friend class STable;

	public:

		f64 GetField(std::string const & Field) const;
		f64 & GetField(std::string const & Field);

		vec3d GetPosition() const;

		SRow operator = (SRow const & Other);

	protected:
		
		SRow(STable & table, u32 const index);

		u32 Index;
		STable & Table;

	};

	class SFieldIterator
	{

	public:

		typedef std::vector<SRow>::iterator::iterator_category iterator_category;
		typedef double value_type;
		typedef std::vector<SRow>::iterator::difference_type difference_type;
		typedef double * pointer;
		typedef double & reference;

		SFieldIterator(std::vector<SRow>::iterator iterator, std::string const & field);

		SFieldIterator & operator ++ ();
		SFieldIterator operator ++ (int);

		f64 operator * () const;
		f64 & operator * ();
		f64 const * operator -> () const;
		f64 * operator -> ();

		bool operator < (SFieldIterator const & other) const;
		bool operator != (SFieldIterator const & other) const;

	protected:

		std::vector<SRow>::iterator Iterator;
		std::string Field;

	};

	STable()
	{}

	vec3d const GetDataScale();

	std::vector<SRow> const & GetValues() const;
	std::vector<SRow> const & GetRows() const;

	SRow & AddRow();
	void AddField(std::string const & Field);
	bool HasField(std::string const & Field);

	//! \param OutlierCutoff Values outside of this many standard deviations are excluded
	Range GetFieldRange(std::string const & Field, double const OutlierCutoff = 5, Range const & acceptedValues = FullRange) const;

	SFieldIterator Begin(std::string const & Field);
	SFieldIterator End(std::string const & Field);

	u32 const Size() const;
	void Clear();
	void WriteToFile(std::ofstream & File);
	void ReadFromFile(std::ifstream & File);

	struct STraits
	{
		std::string PositionXField, PositionYField, PositionZField;
		bool InvertY;

		STraits()
			: PositionXField("x"), PositionYField("y"), PositionZField("z"), InvertY(false)
		{}
	};

	STraits Traits;

protected:

	std::vector<SRow> Rows;
	std::map<std::string, std::vector<double> > Fields;

};

template<class Iterator>
Iterator MaximumRealElement(Iterator First, Iterator Last)
{
	Iterator Found = First;
	if (First != Last)
		for (; ++ First != Last; )
			if (*Found != *Found || (*Found < *First && *First == *First))
				Found = First;
	return Found;
}

template<class Iterator>
Iterator MinimumRealElement(Iterator First, Iterator Last)
{
	Iterator Found = First;
	if (First != Last)
		for (; ++ First != Last; )
			if (*Found != *Found || (*First < *Found && *First == *First))
				Found = First;
	return Found;
}
