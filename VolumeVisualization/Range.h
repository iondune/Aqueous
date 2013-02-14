#ifndef RANGE_H_INCLUDED_
#define RANGE_H_INCLUDED_

#include <ionTypes.h>


typedef std::pair<double, double> Range;
static Range const FullRange = Range(-std::numeric_limits<double>::max(), std::numeric_limits<double>::max());

bool const inRange(double const v, Range const & r);

#endif
