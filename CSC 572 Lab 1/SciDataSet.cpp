#include "SciDataSet.h"

void SciDataSet::setDataScale(Vector3 const & v)
{
	normalizeField("x", v.X);
	normalizeField("y", v.Y);
	normalizeField("z", v.Z);

	DataScale = v;
}

void SciDataSet::normalizeField(std::string const & Field, double const Scale)
{
	if (! (begin(Field) != end(Field)))
		return;

	double max = * max_element_real(begin(Field), end(Field)), min = * min_element_real(begin(Field), end(Field));
	std::for_each(begin(Field), end(Field), [min, max, Scale](double & d) { d = (d - min) / (max - min) * Scale; });
}

SciDataIterator SciDataSet::begin(std::string const & Field)
{
	return SciDataIterator(Values.begin(), Field);
}

SciDataIterator SciDataSet::end(std::string const & Field)
{
	return SciDataIterator(Values.end(), Field);
}
