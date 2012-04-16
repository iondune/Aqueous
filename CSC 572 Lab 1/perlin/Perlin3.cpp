/* coherent noise function over 1, 2 or 3 dimensions */
/* (copyright Ken Perlin) */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "perlin3.h"

#define N 0x1000

#define s_curve(t) ( t * t * (3.0 - 2.0 * t) )
#define lerp(t, a, b) ( a + t * (b - a) )

#define setup(i,b0,b1,r0,r1)\
	t = vec[i] + N;\
	b0 = ((int)t) & BM;\
	b1 = (b0+1) & BM;\
	r0 = t - (int)t;\
	r1 = r0 - 1.0;

f64 Perlin::noise2(f64 vec[2])
{
	s32 bx0, bx1, by0, by1, b00, b10, b01, b11;
	f64 rx0, rx1, ry0, ry1, *q, sx, sy, a, b, t, u, v;
	s32 i, j;

	if (mStart)
	{
		srand(mSeed);
		mStart = false;
		init();
	}

	setup(0,bx0,bx1,rx0,rx1);
	setup(1,by0,by1,ry0,ry1);

	i = p[bx0];
	j = p[bx1];

	b00 = p[i + by0];
	b10 = p[j + by0];
	b01 = p[i + by1];
	b11 = p[j + by1];

	sx = s_curve(rx0);
	sy = s_curve(ry0);

#define at2(rx,ry) ( rx * q[0] + ry * q[1] )

	q = &g2[b00][0];
	u = at2(rx0,ry0);
	q = &g2[b10][0];
	v = at2(rx1,ry0);
	a = lerp(sx, u, v);

	q = &g2[b01][0];
	u = at2(rx0,ry1);
	q = &g2[b11][0];
	v = at2(rx1,ry1);
	b = lerp(sx, u, v);

	return lerp(sy, a, b);
}

f64 Perlin::noise3(f64 vec[2])
{
	s32 bx0, bx1, by0, by1, bz0, bz1, b00, b10, b01, b11;
	f64 rx0, rx1, ry0, ry1, rz0, rz1, *q, sy, sz, a, b, c, d, t, u, v;
	s32 i, j;

	if (mStart)
	{
		srand(mSeed);
		mStart = false;
		init();
	}

	setup(0, bx0,bx1, rx0,rx1);
	setup(1, by0,by1, ry0,ry1);
	setup(2, bz0,bz1, rz0,rz1);

	i = p[ bx0 ];
	j = p[ bx1 ];

	b00 = p[ i + by0 ];
	b10 = p[ j + by0 ];
	b01 = p[ i + by1 ];
	b11 = p[ j + by1 ];

	t  = s_curve(rx0);
	sy = s_curve(ry0);
	sz = s_curve(rz0);

#define at3(rx,ry,rz) ( rx * q[0] + ry * q[1] + rz * q[2] )

	q = g3[ b00 + bz0 ] ; u = at3(rx0,ry0,rz0);
	q = g3[ b10 + bz0 ] ; v = at3(rx1,ry0,rz0);
	a = lerp(t, u, v);

	q = g3[ b01 + bz0 ] ; u = at3(rx0,ry1,rz0);
	q = g3[ b11 + bz0 ] ; v = at3(rx1,ry1,rz0);
	b = lerp(t, u, v);

	c = lerp(sy, a, b);

	q = g3[ b00 + bz1 ] ; u = at3(rx0,ry0,rz1);
	q = g3[ b10 + bz1 ] ; v = at3(rx1,ry0,rz1);
	a = lerp(t, u, v);

	q = g3[ b01 + bz1 ] ; u = at3(rx0,ry1,rz1);
	q = g3[ b11 + bz1 ] ; v = at3(rx1,ry1,rz1);
	b = lerp(t, u, v);

	d = lerp(sy, a, b);

	return lerp(sz, c, d);
}

void Perlin::normalize2(f64 v[2])
{
	f64 s;

	s = (f64)sqrt(v[0] * v[0] + v[1] * v[1]);
	s = 1.0/s;
	v[0] = v[0] * s;
	v[1] = v[1] * s;
}

void Perlin::init()
{
	s32 i, j, k;

	for (i = 0 ; i < B ; i++)
	{
		p[i] = i;
		g1[i] = (f64)((rand() % (B + B)) - B) / B;
		for (j = 0 ; j < 2 ; j++)
		g2[i][j] = (f64)((rand() % (B + B)) - B) / B;
			normalize2(& g2[i][0]);
	}

	while (--i)
	{
		k = p[i];
		p[i] = p[j = rand() % B];
		p[j] = k;
	}

	for (i = 0 ; i < B + 2 ; i++)
	{
		p[B + i] = p[i];
		g1[B + i] = g1[i];
		for (j = 0 ; j < 2 ; j++)
			g2[B + i][j] = g2[i][j];
	}
}

f64 Perlin::perlin_noise_2D(f64 vec[2])
{
	s32 const terms = mOctaves;
	f64 const freq = mFrequency;
	f64 amp = mAmplitude;
	f64 result = 0.0f;

	vec[0] *= mFrequency;
	vec[1] *= mFrequency;

	for(s32 i = 0; i < terms; i ++)
	{
		result += noise2(vec)*amp;

		vec[0] *= 2.0;
		vec[1] *= 2.0;

		vec[0] = fmod(vec[0], (f64)SampleSize);
		vec[1] = fmod(vec[1], (f64)SampleSize);

		amp *= 0.5;
	}

	return result;
}

f64 Perlin::perlin_noise_3D(f64 vec[3])
{
	s32 const terms = mOctaves;
	f64 const freq = mFrequency;
	f64 amp = mAmplitude;
	f64 result = 0.0f;

	vec[0] *= mFrequency;
	vec[1] *= mFrequency;
	vec[2] *= mFrequency;

	for(s32 i = 0; i < terms; i ++)
	{
		result += noise3(vec)*amp;

		vec[0] *= 2.0;
		vec[1] *= 2.0;
		vec[2] *= 2.0;

		vec[0] = fmod(vec[0], (f64)SampleSize);
		vec[1] = fmod(vec[1], (f64)SampleSize);
		vec[2] = fmod(vec[1], (f64)SampleSize);

		amp *= 0.5;
	}

	return result;
}

Perlin::Perlin(s32 sampleSize, s32 octaves, f64 freq, f64 amp, u32 seed)
	: B(sampleSize), BM(sampleSize - 1)
{
	SampleSize = sampleSize;
	mOctaves = octaves;
	mFrequency = freq;
	mAmplitude = amp;
	mSeed = seed;
	mStart = true;

	p = new s32[SampleSize + SampleSize + 2];
	g3 = new f64[SampleSize + SampleSize + 2][3];
	g2 = new f64[SampleSize + SampleSize + 2][2];
	g1 = new f64[SampleSize + SampleSize + 2];
}

Perlin::~Perlin()
{
	delete[] p;
	delete[] g2;
	delete[] g1;
}
