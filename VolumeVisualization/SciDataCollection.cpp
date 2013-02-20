#include "SciDataCollection.h"

#include <fstream>


void SciDataCollection::addData(SciData & Data)
{
	Data.InternalIndex = DataCounter ++;
	Data.ContainingSet = this;
	Values.push_back(Data);

	for (auto it = Fields.begin(); it != Fields.end(); ++ it)
		it->second.resize(max(DataCounter, it->second.size()), 0);
}

void SciDataCollection::addData(SciData & Data, s32 const internalIndex)
{
	Data.InternalIndex = internalIndex;
	DataCounter = max<u32>(internalIndex+1, DataCounter);
	Data.ContainingSet = this;
	Values.push_back(Data);

	for (auto it = Fields.begin(); it != Fields.end(); ++ it)
		it->second.resize(max(DataCounter, it->second.size()), 0);
}

void SciDataCollection::rescaleData(vec3d const & v)
{
	normalizeField(Traits.PositionXField, v.X);
	normalizeField(Traits.PositionYField, v.Y);
	normalizeField(Traits.PositionZField, v.Z);
}

vec3d const SciDataCollection::getDataScale()
{
	Range xRange = getValueRange(Traits.PositionXField),
		yRange = getValueRange(Traits.PositionYField),
		zRange = getValueRange(Traits.PositionZField);

	return vec3d(xRange.second - xRange.first,
		yRange.second - yRange.first,
		zRange.second - zRange.first);
}

void SciDataCollection::normalizeField(std::string const & Field, double const Scale)
{
	if (! (begin(Field) != end(Field)))
		return;

	double max = * max_element_real(begin(Field), end(Field)), min = * min_element_real(begin(Field), end(Field));
	std::for_each(begin(Field), end(Field), [min, max, Scale](double & d) { d = (d - min) / (max - min) * Scale; });
}

Range SciDataCollection::getValueRange(std::string const & Field, double const OutlierCutoff, Range const & acceptedValues)
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

	double StdDeviation = 0;
	for (auto it = DataCopy.begin(); it != DataCopy.end(); ++ it)
	{
		double const v = it->getField(Field);
		if (inRange(v, acceptedValues) && v == v)
			StdDeviation += /*sq*/abs(v - Mean);
	}
	StdDeviation /= (double) (Count - 1);

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
	}

	return std::pair<double, double>(Min, Max);
}

SciDataIterator SciDataCollection::begin(std::string const & Field)
{
	return SciDataIterator(Values.begin(), Field);
}

SciDataIterator SciDataCollection::end(std::string const & Field)
{
	return SciDataIterator(Values.end(), Field);
}

std::vector<SciData> const & SciDataCollection::getValues() const
{
	return Values;
}

u32 const SciDataCollection::size() const
{
	return Values.size();
}

void SciDataCollection::clear()
{
	Values.clear();
	DataCounter = 0;
}

void SciDataCollection::writeToFile(std::ofstream & File)
{
	// Write Fields
	u32 Dims = Fields.size();
	File.write((char *) & Dims, sizeof(u32));

	for (auto it = Fields.begin(); it != Fields.end(); ++ it)
	{
		// Write Label
		Dims = it->first.size();
		File.write((char *) & Dims, sizeof(u32));
		File.write(it->first.c_str(), Dims);

		// Write Data
		Dims = it->second.size();
		File.write((char *) & Dims, sizeof(u32));
		File.write((char *) & it->second.front(), Dims * sizeof(f64));
	}
	
	// Write Values
	Dims = Values.size();
	File.write((char *) & Dims, sizeof(u32));

	for (auto it = Values.begin(); it != Values.end(); ++ it)
	{
		File.write((char *) & it->getPosition().X, sizeof(f64));
		File.write((char *) & it->getPosition().Y, sizeof(f64));
		File.write((char *) & it->getPosition().Z, sizeof(f64));
		File.write((char *) & it->InternalIndex, sizeof(u32));
	}
}

void SciDataCollection::readFromFile(std::ifstream & File)
{
	u32 FieldCount;
	File.read((char *) & FieldCount, sizeof(u32));

	for (u32 i = 0; i < FieldCount; ++ i)
	{
		// Read Label
		u32 LabelLength;
		File.read((char *) & LabelLength, sizeof(u32));
		char * Buffer = new char[LabelLength + 1];
		File.read(Buffer, LabelLength);
		Buffer[LabelLength] = '\0';

		// Read Data
		u32 DataLength;
		File.read((char *) & DataLength, sizeof(u32));
		f64 * DataBuffer = new f64[DataLength];
		File.read((char *) DataBuffer, DataLength * sizeof(double));
		Fields[Buffer].reserve(DataLength);
		for (u32 t = 0; t < DataLength; ++ t)
			Fields[Buffer].push_back(DataBuffer[t]);

		delete [] Buffer;
		delete [] DataBuffer;
	}
	
	// Read Values
	u32 ValueCount;
	File.read((char *) & ValueCount, sizeof(u32));
	Values.reserve(ValueCount);

	for (u32 i = 0; i < ValueCount; ++ i)
	{
		s32 InternalIndex;
		double X, Y, Z;

		File.read((char *) & X, sizeof(f64));
		File.read((char *) & Y, sizeof(f64));
		File.read((char *) & Z, sizeof(f64));
		File.read((char *) & InternalIndex, sizeof(s32));

		SciData d(*this, InternalIndex);
		//d.addField("x") = X;
		//d.addField("y") = Y;
		//d.addField("z") = Z;
		//Values.push_back(d);
	}
}
