#ifndef _CABBAGE_CORE_SBOUNDINGBOX2_H_INCLUDED_
#define _CABBAGE_CORE_SBOUNDINGBOX2_H_INCLUDED_

#include "SVector3.h"
#include "SLine3.h"

class SBoundingBox3
{

public:

    SVector3 MinCorner, MaxCorner;

    SBoundingBox3()
    {}

    SBoundingBox3(SVector3 const & min, SVector3 const & max)
        : MinCorner(min), MaxCorner(max)
    {}

    SBoundingBox3(SVector3 const & v)
        : MinCorner(v), MaxCorner(v)
    {}

    SVector3 const getExtent() const
    {
        return MaxCorner - MinCorner;
    }

    SVector3 const getCenter() const
    {
        return (MaxCorner + MinCorner) / 2.f;
    }

    bool const intersects(SBoundingBox3 const & r) const
    {
        return (MaxCorner.Y > r.MinCorner.Y &&
            MinCorner.Y < r.MaxCorner.Y &&
            MaxCorner.X > r.MinCorner.X &&
            MinCorner.X < r.MaxCorner.X &&
            MaxCorner.Z > r.MinCorner.Z &&
            MinCorner.Z < r.MaxCorner.Z);
    }

    void addInternalPoint(SVector3 const & v)
    {
        if (v.X > MaxCorner.X)
            MaxCorner.X = v.X;
        if (v.Y > MaxCorner.Y)
            MaxCorner.Y = v.Y;
        if (v.Z > MaxCorner.Z)
            MaxCorner.Z = v.Z;

        if (v.X < MinCorner.X)
            MinCorner.X = v.X;
        if (v.Y < MinCorner.Y)
            MinCorner.Y = v.Y;
        if (v.Z < MinCorner.Z)
            MinCorner.Z = v.Z;
    }

	void addInternalBox(SBoundingBox3 const & bb)
	{
		addInternalPoint(bb.MaxCorner);
		addInternalPoint(bb.MinCorner);
	}

    // These intersect methods direct copies from irrlicht engine
    bool intersectsWithLine(SLine3 const & line) const
    {
        return intersectsWithLine(line.getMiddle(), line.getVector().getNormalized(), line.length() * 0.5f);
    }

    // These intersect methods direct copies from irrlicht engine
    bool intersectsWithLine(SVector3 const & linemiddle, SVector3 const & linevect, float halflength) const
    {
        const SVector3 e = getExtent() * 0.5f;
        const SVector3 t = getCenter() - linemiddle;

        if ((fabs(t.X) > e.X + halflength * fabs(linevect.X)) ||
            (fabs(t.Y) > e.Y + halflength * fabs(linevect.Y)) ||
            (fabs(t.Z) > e.Z + halflength * fabs(linevect.Z)))
            return false;

        float r = e.Y * fabs(linevect.Z) + e.Z * fabs(linevect.Y);
        if (fabs(t.Y*linevect.Z - t.Z*linevect.Y) > r)
            return false;

        r = e.X * fabs(linevect.Z) + e.Z * fabs(linevect.X);
        if (fabs(t.Z*linevect.X - t.X*linevect.Z) > r)
            return false;

        r = e.X * fabs(linevect.Y) + e.Y * fabs(linevect.X);
        if (fabs(t.X*linevect.Y - t.Y*linevect.X) > r)
            return false;

        return true;
    }

	float const getMaximumRadius(SVector3 const Scale) const
    {
        SVector3 const Extents = getExtent() / 2.f;
        return std::max(std::max((Extents*Scale).xy().length(), (Extents*Scale).xz().length()), (Extents*Scale).yz().length());
    }

    SVector3 const getCorner(int const i) const
    {
        SVector3 const Center = getCenter();
        SVector3 const Extent = getExtent() / 2.f;

        switch (i)
        {
        default: case 0: return SVector3(Center.X + Extent.X, Center.Y + Extent.Y, Center.Z + Extent.Z);
                 case 1: return SVector3(Center.X + Extent.X, Center.Y - Extent.Y, Center.Z + Extent.Z);
                 case 2: return SVector3(Center.X + Extent.X, Center.Y + Extent.Y, Center.Z - Extent.Z);
                 case 3: return SVector3(Center.X + Extent.X, Center.Y - Extent.Y, Center.Z - Extent.Z);
                 case 4: return SVector3(Center.X - Extent.X, Center.Y + Extent.Y, Center.Z + Extent.Z);
                 case 5: return SVector3(Center.X - Extent.X, Center.Y - Extent.Y, Center.Z + Extent.Z);
                 case 6: return SVector3(Center.X - Extent.X, Center.Y + Extent.Y, Center.Z - Extent.Z);
                 case 7: return SVector3(Center.X - Extent.X, Center.Y - Extent.Y, Center.Z - Extent.Z);
        };
    }

};

#endif
