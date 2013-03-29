#include "SciDataParser.h"
#include "SciData.h"
#include "SciDataManager.h"

#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include <limits>


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

#pragma warning(suppress: 4996)
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
				else if (Fields[column] == "Date Time GMT+01:00" || Fields[column] == "X")
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

enum class ETimes
{
	Pie = 0,
	Hobo = 1,
	SmartTether = 2
};

void SciDataParserPieSlices::load(std::string const & PieFile, std::string const & HoboFile, std::string const & SmartFile)
{
	std::vector<std::vector<f64>> PieSlices, HoboData, SmartTetherData;
	
	auto SavePieValues = [&PieSlices](std::vector<f64> const & v) {PieSlices.push_back(v);};
	auto SaveHoboValues = [&HoboData](std::vector<f64> const & v) {HoboData.push_back(v);};
	auto SaveSmartTether = [&SmartTetherData](std::vector<f64> const & v) {SmartTetherData.push_back(v);};

	LoadCSVFile(PieFile, SavePieValues);
	LoadCSVFile(HoboFile, SaveHoboValues);
	LoadCSVFile(SmartFile, SaveSmartTether);

	int timeOffsets[] = {0, -15, -15};

	for (auto Slice : PieSlices)
	{
		std::cout << "Slice at angle " << Slice[0] << " has ";

		f64 const PieStart = Slice[1] + timeOffsets[(int) ETimes::Pie], PieEnd = Slice[2] + timeOffsets[(int) ETimes::Pie];

		int count = 0;
		for (auto Hobo : HoboData)
		{
			f64 const HoboTime = Hobo[1] + timeOffsets[(int) ETimes::Hobo];
			if (HoboTime >= PieStart && HoboTime < PieEnd)
			{
				count ++;
				bool found = false;

				struct SSmartData
				{
					f64 Depth;
					f64 Time;
				};

				std::vector<SSmartData> SmartData;

				f64 MaxDepth = -std::numeric_limits<f64>::max(), MinDepth = std::numeric_limits<f64>::max();

				for (auto Smart : SmartTetherData)
				{
					f64 const SmartTime = Smart[37] + timeOffsets[(int) ETimes::SmartTether];

					if (SmartTime >= PieStart && SmartTime < PieEnd)
					{
						SSmartData sd;
						sd.Depth = Smart[24];
						sd.Time = SmartTime;
						SmartData.push_back(sd);

						if (sd.Depth > MaxDepth)
							MaxDepth = sd.Depth;
						if (sd.Depth < MinDepth)
							MinDepth = sd.Depth;
					}
				}

				for (auto Smart : SmartData)
				{
					f64 const SmartTime = Smart.Time;

					if (SmartTime > HoboTime)
					{
						found = true;

						f64 const pi = 3.14159;

						f64 const angle = Slice[0] * pi / 180.0;
						f64 const ratio = (Smart.Depth - MinDepth) / (MaxDepth - MinDepth);
						f64 const distance = sqrt(sq(Slice[6]) - sq(Smart.Depth));
						f64 const radial = (Slice[5]*(ratio) + distance*(1.0 - ratio));

						SciData d(Manager->getRawValues());
						d.addField("time") = HoboTime;
						d.addField("x") = cos(angle)*radial;
						d.addField("z") = sin(angle)*radial;
						d.addField("y") = -Smart.Depth;
						d.addField("low") = Hobo[2];
						d.addField("high") = Hobo[3];
						d.addField("temp") = Hobo[4];

						break;
					}
				}

				if (! found)
					std::cout << "Failed to find smart tether point." << std::endl;
			}
		}
		std::cout << count << " hobo points [descent] and ";
		
		count = 0;
		for (auto Hobo : HoboData)
		{
			if (Hobo[1] >= Slice[3] + timeOffsets[(int) ETimes::Pie] && Hobo[1] < Slice[4] + timeOffsets[(int) ETimes::Pie])
				count ++;
		}
		std::cout << count << " hobo points [ascent]." << std::endl;
		
		count = 0;
		for (auto SmartTether : SmartTetherData)
		{
			if (SmartTether[37] >= Slice[1] + timeOffsets[(int) ETimes::SmartTether] && SmartTether[37] < Slice[2] + timeOffsets[(int) ETimes::SmartTether])
				count ++;
		}
		std::cout << count << " smart tether points [descent] and ";
		count = 0;

		for (auto SmartTether : SmartTetherData)
		{
			if (SmartTether[37] >= Slice[3] + timeOffsets[(int) ETimes::SmartTether] && SmartTether[37] < Slice[4] + timeOffsets[(int) ETimes::SmartTether])
				count ++;
		}
		std::cout << count << " smart tether points [ascent]." << std::endl << std::endl;
	}

	std::cout << std::endl << std::endl;
}
