#ifndef _COXYGENLOCALIZEDCOLORMAPPER_H_INCLUDED_
#define _COXYGENLOCALIZEDCOLORMAPPER_H_INCLUDED_

#include "COxygenColorMapper.h"


class COxygenLocalizedColorMapper : public COxygenColorMapper
{

public:
	
	Range XRange;
	Range YRange;
	Range ZRange;
	
	float EmphasisLocation;
	float LocalRange;
	float MinimumAlpha;

	SVector3f SliceAxis;

	COxygenLocalizedColorMapper()
	{
		EmphasisLocation = 0.5f;
		LocalRange = 0.1f;
		MinimumAlpha = 0.03f;
		SliceAxis = SVector3f(0.f, 1.f, 0.f);
	}

	virtual SColor const getColor(SciData const & d)
	{
		SColor c = COxygenColorMapper::getColor(d);

		float X = (float) ((d.getField("x") - XRange.first) / (XRange.second - XRange.first));
		float Y = (float) ((d.getField("y") - YRange.first) / (YRange.second - YRange.first));
		float Z = (float) ((d.getField("z") - ZRange.first) / (ZRange.second - ZRange.first));

		SVector3f const LocalVector = SVector3f(X, Y, Z) - SVector3f(0.5f);
		SVector3f const PlanarVector = SliceAxis.getNormalized();

		float Expansion;
		SVector3f const MVec(abs(PlanarVector.X), abs(PlanarVector.Y), abs(PlanarVector.Z));
		if (MVec.X > MVec.Y && MVec.X > MVec.Z)
		{
			Expansion = 1.f / MVec.X;
		}
		else if (MVec.Y > MVec.X && MVec.Y > MVec.Z)
		{
			Expansion = 1.f / MVec.Y;
		}
		else if (MVec.Z > MVec.X && MVec.Z > MVec.Y)
		{
			Expansion = 1.f / MVec.Z;
		}

		float const Distance = abs(((LocalVector / Expansion).dotProduct(PlanarVector) + 0.5f) - EmphasisLocation);

		if (Distance < LocalRange / 2.f)
		{
			float Ratio = 1.f - Distance / (LocalRange / 2.f);
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
		XRange = s.getValueRange("x", 5.0);
		YRange = s.getValueRange("y", 5.0);
		ZRange = s.getValueRange("z", 5.0);
	}

};

#endif
