#ifndef PERLIN_H_
#define PERLIN_H_

#include <stdlib.h>
#include <vector>

typedef int s32;
typedef double f64;
typedef unsigned int u32;


class Perlin
{

public:

	Perlin(s32 sampleSize, s32 octaves, f64 freq, f64 amp, u32 seed);
	~Perlin();

	f64 Get(f64 x, f64 y, bool limit)
	{
		if (limit && (x < 0 || x >= SampleSize || y < 0 || y >= SampleSize))
			return 0;
		f64 vec[2];
		vec[0] = x;
		vec[1] = y;
		return perlin_noise_2D(vec);
	};

	f64 Get(f64 x, f64 y, f64 z, bool limit)
	{
		if (limit && (x < 0 || x >= SampleSize || y < 0 || y >= SampleSize || z < 0 || z >= SampleSize))
			return 0;

		f64 vec[3];
		vec[0] = x;
		vec[1] = y;
		vec[2] = z;
		return perlin_noise_3D(vec);
	};

	int SampleSize;

private:

	f64 perlin_noise_2D(f64 vec[2]);
	f64 perlin_noise_3D(f64 vec[3]);

	f64 noise2(f64 vec[2]);
	f64 noise3(f64 vec[3]);
	void normalize2(f64 v[2]);
	void init();

	s32 mOctaves;
	f64 mFrequency;
	f64 mAmplitude;
	u32 mSeed;

	s32 const B;
	s32 const BM;

	s32 * p;
	f64 (* g3)[3];
	f64 (* g2)[2];
	f64 * g1;
	bool  mStart;

};

#endif
