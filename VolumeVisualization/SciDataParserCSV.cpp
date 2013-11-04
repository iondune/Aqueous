
#include "SciDataParser.h"
#include "CDataSet.h"


void SciDataParserCSV::Load(std::string const & FileName)
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
			while (Stream.good())
			{
				std::string Label;
				std::getline(Stream, Label, FieldDelim);
				Fields.push_back(Label);
			}
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

			u32 Length = std::min(Row.size(), Fields.size());

			STable::SRow & TableRow = DataSet->Points.AddRow();
			for (u32 i = 0; i < Length; ++ i)
				TableRow.GetField(Fields[i]) = Row[i];
		}
	}
}

void SciDataParserCSV::LoadAdditional(std::string const & FileName)
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
			std::vector<std::string> TestFields;
			while (Stream.good())
			{
				std::string Label;
				std::getline(Stream, Label, FieldDelim);
				TestFields.push_back(Label);
			}
			if (TestFields != Fields)
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

			u32 Length = std::min(Row.size(), Fields.size());

			STable::SRow & TableRow = DataSet->Points.AddRow();
			for (u32 i = 0; i < Length; ++ i)
				TableRow.GetField(Fields[i]) = Row[i];
		}
	}
}
