#include "SciDataManager.h"

#include <iostream>
#include <fstream>


void SciDataManager::writeToFile(std::string const & FileName)
{
	std::ofstream File(FileName, std::ios::out | std::ios::binary);

	if (File.is_open())
	{
		u32 Dims = GridDimensions ? 3 : 0;
		File.write((char *) & Dims, 4);
		for (u32 i = 0; i < Dims; ++ i)
		{
			Dims = GridDimensions[i];
			File.write((char *) & Dims, 4);
		}

		Dims = GridValues.size();
		File.write((char *) & Dims, 4);
		for (auto it = GridValues.Values.begin(); it != GridValues.Values.end(); ++ it)
		{
			File.write((char *) & it->Location.X, sizeof(double));
			File.write((char *) & it->Location.Y, sizeof(double));
			File.write((char *) & it->Location.Z, sizeof(double));

			Dims = it->ScalarFields.size();
			File.write((char *) & Dims, 4);
			for (auto jt = it->ScalarFields.begin(); jt != it->ScalarFields.end(); ++ jt)
			{
				Dims = jt->first.length();
				File.write((char *) & Dims, 4);
				File.write(jt->first.c_str(), Dims);
				File.write((char *) & jt->second, sizeof(double));
			}
		}

		Dims = RawValues.size();
		File.write((char *) & Dims, 4);
		for (auto it = RawValues.Values.begin(); it != RawValues.Values.end(); ++ it)
		{
			File.write((char *) & it->Location.X, sizeof(double));
			File.write((char *) & it->Location.Y, sizeof(double));
			File.write((char *) & it->Location.Z, sizeof(double));

			Dims = it->ScalarFields.size();
			File.write((char *) & Dims, 4);
			for (auto jt = it->ScalarFields.begin(); jt != it->ScalarFields.end(); ++ jt)
			{
				Dims = jt->first.length();
				File.write((char *) & Dims, 4);
				File.write(jt->first.c_str(), Dims);
				File.write((char *) & jt->second, sizeof(double));
			}
		}
	}

	File.close();
}

void SciDataManager::readFromFile(std::string const & FileName)
{
	std::ifstream File(FileName, std::ios::out | std::ios::binary);

	if (File.is_open())
	{
		u32 Dims;
		File.read((char *) & Dims, 4);

		if (Dims)
			GridDimensions = new int[Dims];
		for (u32 i = 0; i < Dims; ++ i)
		{
			File.read((char *) & Dims, 4);
			GridDimensions[i] = Dims;
		}
		
		File.read((char *) & Dims, 4);
		u32 DataCount = Dims;
		for (u32 i = 0; i < DataCount; ++ i)
		{
			SciData d;
			File.read((char *) & d.Location.X, sizeof(double));
			File.read((char *) & d.Location.Y, sizeof(double));
			File.read((char *) & d.Location.Z, sizeof(double));

			File.read((char *) & Dims, 4);
			u32 FieldCount = Dims;
			for (u32 j = 0; j < FieldCount; ++ j)
			{
				File.read((char *) & Dims, 4);
				char * Buffer = new char[Dims + 1];
				File.read(Buffer, Dims);
				Buffer[Dims] = '\0';

				double Value;
				File.read((char *) & Value, sizeof(double));

				d.ScalarFields[std::string(Buffer)] = Value;
			}

			GridValues.Values.push_back(d);
		}
		
		File.read((char *) & Dims, 4);
		DataCount = Dims;
		for (u32 i = 0; i < DataCount; ++ i)
		{
			SciData d;
			File.read((char *) & d.Location.X, sizeof(double));
			File.read((char *) & d.Location.Y, sizeof(double));
			File.read((char *) & d.Location.Z, sizeof(double));

			File.read((char *) & Dims, 4);
			u32 FieldCount = Dims;
			for (u32 j = 0; j < FieldCount; ++ j)
			{
				File.read((char *) & Dims, 4);
				char * Buffer = new char[Dims + 1];
				File.read(Buffer, Dims);
				Buffer[Dims] = '\0';

				double Value;
				File.read((char *) & Value, sizeof(double));

				d.ScalarFields[std::string(Buffer)] = Value;
			}

			RawValues.Values.push_back(d);
		}
	}

	File.close();
}
