
#include "SciDataParser.h"


f64 const string_to_double(std::string const & s)
{
	std::istringstream iss(s);
	f64 t;
	iss >> t;
	return t;
}
