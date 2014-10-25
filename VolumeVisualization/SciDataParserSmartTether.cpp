
#include "SciDataParser.h"
#include "CDataSet.h"

#include <fstream>
#include <sstream>


void SciDataParserSmartTether::load(std::string const & FileName)
{
	std::ifstream File;
	File.open(FileName);
	bool FirstLine = FieldNames;

	while (File.is_open() && File.good())
	{
		std::string Line;
		std::getline(File, Line);

		if (Line == "")
			continue;

		std::istringstream Stream(Line);

		if (FirstLine)
		{
			while (Stream.good())
			{
				std::string Label;
				std::getline(Stream, Label, FieldDelim);
				Fields.push_back(Label);
			}
			Fields[16] = "time";
			Fields[17] = "z";
			Fields[18] = "z_cardinal";
			Fields[19] = "x";
			Fields[20] = "x_cardinal";
			Fields[24] = "y";
			Fields.resize(38);
			FirstLine = false;
		}
		else
		{
			std::vector<f64> Row;
			while (Stream.good())
			{
				std::string Label;
				std::getline(Stream, Label, ValueDelim);
				Row.push_back(string_to_double(Label));
			}

			if (Row.size() != Fields.size())
				std::cerr << "Mismatched row size at row " << DataSet->Points.Size() << ", found " << Row.size() << " but expected " << Fields.size() << std::endl;
			size_t Length = Min(Row.size(), Fields.size());

			STable::SRow & TableRow = DataSet->Points.AddRow();
			for (u32 i = 0; i < Length; ++ i)
				TableRow.GetField(Fields[i]) = Row[i];
		}
	}
}
