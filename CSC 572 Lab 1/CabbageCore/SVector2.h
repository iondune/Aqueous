#ifndef _CABBAGE_CORE_SVECTOR2_H_INCLUDED_
#define _CABBAGE_CORE_SVECTOR2_H_INCLUDED_

#include <cmath>
#include <stdio.h>

class SVector2
{

public:

	float X, Y;

	SVector2()
		: X(0), Y(0)
	{}

	SVector2(float in)
		: X(in), Y(in)
	{}

	SVector2(float in_x, float in_y)
		: X(in_x), Y(in_y)
	{}

	float const operator[] (int i) const
	{
		return (i == 0 ? X : Y);
	}

	float & operator[] (int i)
	{
		return (i == 0 ? X : Y);
	}

	void reset()
	{
		X = 0;
		Y = 0;
	}

	void set(float in)
	{
		X = in;
		Y = in;
	}

	void set(float in_x, float in_y)
	{
		X = in_x;
		Y = in_y;
	}

	float dotProduct(SVector2 const & v) const
	{
		return X*v.X + Y*v.Y;
	}

	float length() const
	{
		return sqrtf(X*X + Y*Y);
	}

	SVector2 operator + (SVector2 const & v) const
	{
		return SVector2(X+v.X, Y+v.Y);
	}

	SVector2 & operator += (SVector2 const & v)
	{
		X += v.X;
		Y += v.Y;

		return * this;
	}

	SVector2 operator - (SVector2 const & v) const
	{
		return SVector2(X-v.X, Y-v.Y);
	}

	SVector2 & operator -= (SVector2 const & v)
	{
		X -= v.X;
		Y -= v.Y;

		return * this;
	}

	SVector2 operator * (SVector2 const & v) const
	{
		return SVector2(X*v.X, Y*v.Y);
	}

	SVector2 & operator *= (SVector2 const & v)
	{
		X *= v.X;
		Y *= v.Y;

		return * this;
	}

	SVector2 operator / (SVector2 const & v) const
	{
		return SVector2(X/v.X, Y/v.Y);
	}

	SVector2 & operator /= (SVector2 const & v)
	{
		X /= v.X;
		Y /= v.Y;

		return * this;
	}

	SVector2 operator * (float const s) const
	{
		return SVector2(X*s, Y*s);
	}

	SVector2 & operator *= (float const s)
	{
		X *= s;
		Y *= s;

		return * this;
	}

	SVector2 operator / (float const s) const
	{
		return SVector2(X/s, Y/s);
	}

	SVector2 & operator /= (float const s)
	{
		X /= s;
		Y /= s;

		return * this;
	}

};


#endif
