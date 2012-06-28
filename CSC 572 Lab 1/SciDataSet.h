#ifndef _SCI_DATA_SET_H_INCLUDED_
#define _SCI_DATA_SET_H_INCLUDED_

#include "SciData.h"
#include "SciDataIterator.h"


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
		if (! (begin(Field) != end(Field)))
			return;

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

#endif
