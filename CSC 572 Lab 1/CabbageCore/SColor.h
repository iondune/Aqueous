#ifndef _CABBAGE_CORE_SCOLOR_H_INCLUDED_
#define _CABBAGE_CORE_SCOLOR_H_INCLUDED_

#include "SVector3.h"

class SColor
{

public:

    float Red, Green, Blue, Alpha;

    SColor()
        : Red(0.7f), Green(0.7f), Blue(0.7f), Alpha(1.f)
    {}

	SColor(float red, float green, float blue)
        : Red(red), Green(green), Blue(blue), Alpha(1.f)
    {}

	SColor(float red, float green, float blue, float alpha)
        : Red(red), Green(green), Blue(blue), Alpha(alpha)
    {}

	SColor(SVector3 const & vector)
		: Red(vector.X), Green(vector.Y), Blue(vector.Z), Alpha(1.f)
	{}

	float const operator[] (unsigned int i) const
	{
		switch (i)
		{
		default:
		case 0:
			return Red;
		case 1:
			return Green;
		case 2:
			return Blue;
		case 3:
			return Alpha;
		}
	}

	float & operator[] (unsigned int i)
	{
		switch (i)
		{
		default:
		case 0:
			return Red;
		case 1:
			return Green;
		case 2:
			return Blue;
		case 3:
			return Alpha;
		}
	}

};

#endif
