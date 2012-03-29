#ifndef _CABBAGE_CORE_SLINE3_H_INCLUDED_
#define _CABBAGE_CORE_SLINE3_H_INCLUDED_

#include "SVector3.h"

class SLine3
{

public:

    SVector3 Start, End;

    SLine3()
    {}

    SLine3(SVector3 const & start, SVector3 const & end)
        : Start(start), End(end)
    {}

    SVector3 const getMiddle() const
    {
        return (Start + End) / 2.f;;;
    }

    SVector3 const getVector() const
    {
        return Start - End;
    }

    float const length() const
    {
        return getVector().length();
    }

};

#endif
