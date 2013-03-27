#include "SciDataParser.h"

#include <fstream>
#include <sstream>
#include <string>
#include <iostream>


static f64 const string_to_double(std::string const & s)
{
	std::istringstream iss(s);
	f64 t;
	iss >> t;
	return t;
}

static f64 const time_to_double(std::string const & s)
{
	std::istringstream iss(s);
	f64 hrs, min, sec;
	iss >> hrs;
	iss.ignore(1);
	iss >> min;
	iss.ignore(1);
	iss >> sec;
	return hrs * 60 * 60 + min * 60 + sec;
}

static f64 const length_to_double(std::string const & s)
{
	std::istringstream iss(s);
	f64 feet, inch;
	iss >> feet;
	iss.ignore(1);
	iss >> inch;
	return feet * 12 + inch;
}

static f64 const datetime_to_double(std::string const & s)
{
	int day, month, year, hours, minutes, seconds;
	char ampm;

	if (7 != sscanf(s.c_str(), "%d/%d/%d %d:%d:%d %cM", &day, &month, &year, &hours, &minutes, &seconds, &ampm))
		std::cerr << "SHIT!" << std::endl;

	return (f64) ((hours + (ampm == 'P' ? 12 : 0)) * 60 * 60 + minutes * 60 + seconds);
}

template <typename T>
void LoadCSVFile(std::string const & fileName, T & operation)
{
	std::ifstream File;
	File.open(fileName);
	bool FirstLine = true;

	char FieldDelim = ',', ValueDelim = ',';

	std::vector<std::string> Fields;

	int row = 0;

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
				if (Label.size() && Label.front() == '"' && Label.back() == '"')
					Label = Label.substr(1, Label.size() - 2);
				Fields.push_back(Label);
			}
			FirstLine = false;
		}
		else
		{
			std::vector<f64> Row;
			int column = 0;
			while (Stream.good())
			{
				std::string Label;
				std::getline(Stream, Label, ValueDelim);
				if (Fields[column].substr(0, 4) == "time")
					Row.push_back(time_to_double(Label));
				else if (Fields[column].substr(0, 3) == "len")
					Row.push_back(length_to_double(Label));
				else if (Fields[column] == "Date Time GMT+01:00")
					Row.push_back(datetime_to_double(Label));
				else
					Row.push_back(string_to_double(Label));
				column ++;
			}

			//if (Row.size() != Fields.size())
			//	std::cerr << "Mismatched row size at row " << row << ", found " << Row.size() << " but expected " << Fields.size() << std::endl;
			u32 Length = std::min(Row.size(), Fields.size());

			operation(Row);
			row ++;
		}
	}
}

void SciDataParserPieSlices::load(std::string const & FileName)
{
	std::vector<std::vector<f64>> PieSlices, HoboData, SmartTetherData;
	
	auto SavePieValues = [&PieSlices](std::vector<f64> const & v) {PieSlices.push_back(v);};
	auto SaveHoboValues = [&HoboData](std::vector<f64> const & v) {HoboData.push_back(v);};
	auto SaveSmartTether = [&SmartTetherData](std::vector<f64> const & v) {SmartTetherData.push_back(v);};

	LoadCSVFile("PieSlices.csv", SavePieValues);
	LoadCSVFile("StGeorgesBayCaveDay1Deployment2.csv", SaveHoboValues);
	LoadCSVFile("2013_03_26_02_48_27.csv", SaveSmartTether);

	for (auto Slice : PieSlices)
	{
		std::cout << "Slice at angle " << Slice[0] << " has ";

		int count = 0;
		for (auto Hobo : HoboData)
		{
			if (Hobo[1] >= Slice[1] && Hobo[1] < Slice[2])
				count ++;
		}
		std::cout << count << " hobo points for descent and ";
		
		count = 0;
		for (auto Hobo : HoboData)
		{
			if (Hobo[1] >= Slice[3] && Hobo[1] < Slice[4])
				count ++;
		}
		std::cout << count << " hobo points for ascent." << std::endl;
	}
}
