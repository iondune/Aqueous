#include "SciDataManager.h"

#include <iostream>
#include <fstream>


void SciDataManager::writeToFile(std::string const & FileName)
{
	std::ofstream File(FileName, std::ios::out | std::ios::binary);

	if (File.is_open())
	{
		u32 Dims = GridDimensions ? 3 : 0;
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

		if (Dims)
			GridDimensions = new int[Dims];
		for (u32 i = 0; i < Dims; ++ i)
		{
			int Value;
			File.read((char *) & Value, sizeof(u32));
			GridDimensions[i] = Value;
		}
		
		GridValues.readFromFile(File);
		RawValues.readFromFile(File);
	}

	File.close();
}
