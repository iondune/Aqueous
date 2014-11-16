
#include "SciDataParser.h"
#include "CDataSet.h"


void SciDataParserCSV::Load()
{
	DataSet->SourceFile = FileName;

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
			std::vector<std::string> ReadFields;
			while (Stream.good())
			{
				std::string Label;
				std::getline(Stream, Label, FieldDelim);
				ReadFields.push_back(Label);
			}
			if (Fields.size() == 0)
				Fields = ReadFields;
			else if (ReadFields != Fields)
				std::cerr << "Mismatched field headers in CSV files." << std::endl;
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
				std::cerr << "Mismatched row size at row " << DataSet->Points.Size() << " in file '" << FileName << "', found " << Row.size() << " but expected " << Fields.size() << std::endl;

			size_t Length = std::min(Row.size(), Fields.size());

			STable::SRow & TableRow = DataSet->Points.AddRow();
			for (u32 i = 0; i < Length; ++ i)
				TableRow.GetField(Fields[i]) = Row[i];
		}
	}
}
