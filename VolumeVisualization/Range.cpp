#include "Range.h"


bool const InRange(double const v, Range const & r)
{
	return v >= r.first && v <= r.second;
}
