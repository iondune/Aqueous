#include "SciDataParser.h"

#include <iostream>
#include <fstream>


void SciDataParser::writeToFile(std::string const & FileName)
{
	std::ofstream File(FileName, std::ios::out | std::ios::binary);

	if (File.is_open())
	{

	}
}

void SciDataParser::readFromFile(std::string const & FileName)
{
}
