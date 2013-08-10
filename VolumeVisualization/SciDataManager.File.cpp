#include "SciDataManager.h"

#include <iostream>
#include <fstream>


void SciDataManager::writeToFile(std::string const & FileName)
{
	std::ofstream File(FileName, std::ios::out | std::ios::binary);

	if (File.is_open())
	{
		u32 Dims = 3;
		File.write((char *) & Dims, sizeof(u32));
		for (u32 i = 0; i < Dims; ++ i)
		{
			int Value = GridDimensions[i];
			File.write((char *) & Value, sizeof(u32));
		}

		GridValues.writeToFile(File);
		RawValues.writeToFile(File);
	}

	File.close();
}

void SciDataManager::readFromFile(std::string const & FileName)
{
	std::ifstream File(FileName, std::ios::out | std::ios::binary);

	if (File.is_open())
	{
		u32 Dims;
		File.read((char *) & Dims, sizeof(u32));

		if (Dims != 3)
		{
			std::cerr << "Unexpected dimensions of grid data (" << Dims << ") in file " << FileName;
			for (u32 i = 0; i < Dims; ++ i)
			{
				int Value;
				File.read((char *) & Value, sizeof(u32));
			}
		}
		else
		{
			for (u32 i = 0; i < 3; ++ i)
				File.read((char *) & GridDimensions[i], sizeof(u32));
		}

		
		GridValues.readFromFile(File);
		RawValues.readFromFile(File);
	}

	File.close();
}

void SciDataManager::readFromFileLegacy(std::string const & FileName)
{
	std::ifstream File(FileName, std::ios::out | std::ios::binary);

	if (File.is_open())
	{
		u32 Dims;
		File.read((char *) & Dims, sizeof(u32));

		for (u32 i = 0; i < Dims; ++ i)
		{
			int Value;
			File.read((char *) & Value, sizeof(u32));
			GridDimensions[i] = Value;
		}
		
		File.read((char *) & Dims, sizeof(u32));
		u32 DataCount = Dims;
		GridValues.Values.reserve(DataCount);
		for (u32 i = 0; i < DataCount; ++ i)
		{
			SciData d(GridValues);
			File.read((char *) & d.getField("x"), sizeof(double));
			File.read((char *) & d.getField("y"), sizeof(double));
			File.read((char *) & d.getField("z"), sizeof(double));

			File.read((char *) & Dims, sizeof(u32));
			u32 FieldCount = Dims;
			for (u32 j = 0; j < FieldCount; ++ j)
			{
				File.read((char *) & Dims, sizeof(u32));
				char * Buffer = new char[Dims + 1];
				File.read(Buffer, Dims);
				Buffer[Dims] = '\0';

				double Value;
				File.read((char *) & Value, sizeof(double));

				d.getField(std::string(Buffer)) = Value;
			}

			GridValues.Values.push_back(d);
		}
		
		File.read((char *) & Dims, sizeof(u32));
		DataCount = Dims;
		RawValues.Values.reserve(DataCount);
		for (u32 i = 0; i < DataCount; ++ i)
		{
			SciData d(RawValues);
			File.read((char *) & d.getField("x"), sizeof(double));
			File.read((char *) & d.getField("y"), sizeof(double));
			File.read((char *) & d.getField("z"), sizeof(double));

			File.read((char *) & Dims, sizeof(u32));
			u32 FieldCount = Dims;
			for (u32 j = 0; j < FieldCount; ++ j)
			{
				File.read((char *) & Dims, sizeof(u32));
				char * Buffer = new char[Dims + 1];
				File.read(Buffer, Dims);
				Buffer[Dims] = '\0';

				double Value;
				File.read((char *) & Value, sizeof(double));
				
				d.getField(std::string(Buffer)) = Value;
			}

			RawValues.Values.push_back(d);
		}
	}

	File.close();
}
