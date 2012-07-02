#ifndef _COXYGENISOSURFACECOLORMAPPER_H_INCLUDED_
#define _COXYGENISOSURFACECOLORMAPPER_H_INCLUDED_

#include "COxygenColorMapper.h"


class COxygenIsoSurfaceColorMapper : public COxygenColorMapper
{

public:

	Range ValueRange;
	
	float EmphasisLocation;
	float LocalRange;
	float MinimumAlpha;

	COxygenIsoSurfaceColorMapper()
		: EmphasisLocation(0.5f)
	{
		LocalRange = 0.1f;
		MinimumAlpha = 0.03f;
	}

	virtual SColor const getColor(SciData const & d)
	{
		double const v = d.getField("o1");
		SColor c = COxygenColorMapper::getColor(d);

		double const Z = (v - ValueRange.first) / (ValueRange.second - ValueRange.first);
		float const Height = (float) Z;

		if (abs(Height - EmphasisLocation) < LocalRange / 2.f)
		{
			float Ratio = 1.f - abs(Height - EmphasisLocation) / (LocalRange / 2.f);
			c.Alpha = Ratio * (1.f - MinimumAlpha) + MinimumAlpha;
		}
		else
		{
			c.Alpha = MinimumAlpha;
		}

		return c;
	}

	virtual void preProcessValues(SciDataSet & s)
	{
		ValueRange = s.getValueRange("o1", 5.0);
	}

};

#endif
