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

template <typename T>
static inline T sq(T const v) { return v * v; }

std::pair<double, double> SciDataSet::getValueRange(std::string const & Field, double const OutlierCutoff)
{
	std::vector<SciData> DataCopy = Values;
	std::sort(DataCopy.begin(), DataCopy.end(), [Field](SciData const & left, SciData const & right) -> bool
	{
		return left.getField(Field) < right.getField(Field);
	});

	double Mean = 0;
	for (auto it = DataCopy.begin(); it != DataCopy.end(); ++ it)
	{
		Mean += it->getField(Field) / (double) (DataCopy.size());
	}

	double StdDeviation = 0;
	for (auto it = DataCopy.begin(); it != DataCopy.end(); ++ it)
	{
		StdDeviation += sq(it->getField(Field) - Mean);
	}

	StdDeviation = sqrt(StdDeviation);

	double Min = std::numeric_limits<double>::max(), Max = -std::numeric_limits<double>::max();

	for (auto it = DataCopy.begin(); it != DataCopy.end(); ++ it)
	{
		double const v = it->getField(Field);

		if (v < Mean + OutlierCutoff * StdDeviation && v > Mean - OutlierCutoff * StdDeviation)
		{
			if (v > Max)
				Max = v;
			if (v < Min)
				Min = v;
		}
	}

	return std::pair<double, double>(Min, Max);
}

SciDataIterator SciDataSet::begin(std::string const & Field)
{
	return SciDataIterator(Values.begin(), Field);
}

SciDataIterator SciDataSet::end(std::string const & Field)
{
	return SciDataIterator(Values.end(), Field);
}
