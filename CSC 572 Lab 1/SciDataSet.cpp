#include "SciDataSet.h"

bool const inRange(double const v, Range const & r)
{
	return v >= r.first && v <= r.second;
}

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

std::pair<double, double> SciDataSet::getValueRange(std::string const & Field, double const OutlierCutoff, Range const & acceptedValues)
{
	std::vector<SciData> const & DataCopy = Values;

	double Mean = 0;
	unsigned int Count = DataCopy.size();
	for (auto it = DataCopy.begin(); it != DataCopy.end(); ++ it)
	{
		double const v = it->getField(Field);
		if (! inRange(v, acceptedValues) || v != v)
			Count --;
		else
			Mean += it->getField(Field);
	}
	Mean /= (double) (Count);
	printf("Data mean is %e with %d exclusions ", Mean, DataCopy.size() - Count);

	double StdDeviation = 0;
	for (auto it = DataCopy.begin(); it != DataCopy.end(); ++ it)
	{
		double const v = it->getField(Field);
		if (inRange(v, acceptedValues) && v == v)
			StdDeviation += /*sq*/abs(v - Mean);
	}
	StdDeviation /= (double) (Count - 1);
	//StdDeviation = sqrt(StdDeviation);
	printf("- stdev is %e\n", StdDeviation);

	double Min = std::numeric_limits<double>::max(), Max = -std::numeric_limits<double>::max();

	for (auto it = DataCopy.begin(); it != DataCopy.end(); ++ it)
	{
		double const v = it->getField(Field);

		if (v == 0.0)
			continue;

		if (v < Mean + OutlierCutoff * StdDeviation && v > Mean - OutlierCutoff * StdDeviation)
		{
			if (v > Max)
				Max = v;
			if (v < Min)
				Min = v;
		}
		else
		{
			//printf("Value %e was excluded from range!\n", v);
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
