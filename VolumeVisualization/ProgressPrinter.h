
#pragma once

#include <stdio.h>


class ProgressPrinter
{

public:

	ProgressPrinter(int const barLength = 30)
		: Progress(-1), BarLength(barLength)
	{}

	void Begin()
	{
		printf("Progress: \n");
		Update(0);
	}

	void Update(int const progress)
	{
		if (progress <= Progress)
			return;

		Progress = progress;
		printf("\r");
		printf("%3d%% :[", Progress);
		int i = 0;
		for (; i < Progress * BarLength / 100; ++ i)
			printf("%c", 0xB2);
		for (; i < BarLength; ++ i)
			printf("%c", 0xB0);
		printf("]");
	}

	void End()
	{
		Update(100);
		printf("\n");
		Progress = 0;
	}

protected:

	int Progress;
	int BarLength;

};
