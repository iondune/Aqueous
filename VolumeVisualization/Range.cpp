#include "Range.h"


bool const inRange(double const v, Range const & r)
{
	return v >= r.first && v <= r.second;
}
