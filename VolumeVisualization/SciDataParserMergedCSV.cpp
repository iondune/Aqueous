
#include "SciDataParser.h"
#include "CDataSet.h"

#include <fstream>
#include <sstream>


u32 const timeToSeconds(std::string const & Field)
{
	u32 In, Out = 0;
	std::istringstream s(Field);
	s >> In;
	Out += In * 60 * 60;
	s.ignore(1, ':');
	s >> In;
	Out += In * 60;
	s.ignore(1, ':');
	s >> In;
	Out += In;
	return Out;
}

void SciDataParserMergedCSV::Load(std::string const & FileName1, std::string const & FileName2, std::string const & MatchField)
{
	std::ifstream File1, File2;
	File1.open(FileName1);
	File2.open(FileName2);
	bool FirstLine = FieldNames;
	bool ReadNext1 = true, ReadNext2 = true;
	std::vector<std::string> Row1, Row2;

	std::vector<std::string> Fields1, Fields2;
	s32 FieldIndex1 = -1, FieldIndex2 = -1;

	while (File1.is_open() && File1.good() && File2.is_open() && File2.is_open() && ! File1.eof() && ! File2.eof())
	{
		std::string Line1;
		if (ReadNext1)
			std::getline(File1, Line1).eof();

		std::string Line2;
		if (ReadNext2)
			std::getline(File2, Line2);

		std::istringstream Stream1(Line1), Stream2(Line2);

		if (FirstLine)
		{
			int Counter = 0;
			while (Stream1.good())
			{
				std::string Label;
				std::getline(Stream1, Label, FieldDelim);
				Fields1.push_back(Label);

				if (Label == MatchField)
					FieldIndex1 = Counter;
			}
			Counter = 0;
			while (Stream2.good())
			{
				std::string Label;
				std::getline(Stream2, Label, FieldDelim);
				Fields2.push_back(Label);

				if (Label == MatchField)
					FieldIndex2 = Counter;
				Counter ++;
			}
			FirstLine = false;
		}
		else
		{
			if (ReadNext1)
			{
				Row1.clear();
				while (Stream1.good())
				{
					std::string Label;
					std::getline(Stream1, Label, ValueDelim);
					Row1.push_back(Label);
				}
				ReadNext1 = false;
			}
			if (ReadNext2)
			{
				Row2.clear();
				while (Stream2.good())
				{
					std::string Label;
					std::getline(Stream2, Label, ValueDelim);
					Row2.push_back(Label);
				}
				ReadNext2 = false;
			}
			
			if (Row1.size() != Fields1.size())
			{
				std::cerr << "Mismatched row size at row " << DataSet->Points.Size() << std::endl;
				ReadNext1 = true;
			}
			int Length1 = (int) std::min(Row1.size(), Fields1.size());

			if (Row2.size() != Fields2.size())
			{
				std::cerr << "Mismatched row size at row " << DataSet->Points.Size() << std::endl;
				ReadNext2 = true;
			}
			int Length2 = (int) std::min(Row2.size(), Fields2.size());

			if (Length1 > FieldIndex1 && Length2 > FieldIndex2)
			{
				u32 Field1 = timeToSeconds(Row1[FieldIndex1]), Field2 = timeToSeconds(Row2[FieldIndex2]);

				if (std::abs((int) (Field1 - Field2)) <= 2)
				{
					STable::SRow & Row = DataSet->Points.AddRow();
					for (s32 i = 0; i < Length1; ++ i)
						Row.GetField(Fields1[i]) = string_to_double(Row1[i]);
					for (s32 i = 0; i < Length2; ++ i)
						if (i != FieldIndex2)
							Row.GetField(Fields2[i]) = string_to_double(Row2[i]);
					ReadNext1 = ReadNext2 = true;
				}
				else if (Field1 > Field2)
				{
					ReadNext2 = true;
				}
				else if (Field2 > Field1)
				{
					ReadNext1 = true;
				}
				else
				{
					std::cerr << "Unexpected value mismatch." << std::endl;
					ReadNext1 = ReadNext2 = true;
				}
			}
		}
	}
}
