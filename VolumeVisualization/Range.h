
#pragma once

#include <ionCore/ionTypes.h>
#include <algorithm>


typedef std::pair<double, double> Range;
static Range const FullRange = Range(-std::numeric_limits<double>::max(), std::numeric_limits<double>::max());

bool const InRange(double const v, Range const & r);
