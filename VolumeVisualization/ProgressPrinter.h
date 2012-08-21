#ifndef _PROGRESS_PRINTER_H_INCLUDED_
#define _PROGRESS_PRINTER_H_INCLUDED_

#include <stdio.h>


class ProgressPrinter
{

	int Progress;
	int BarLength;

public:

	ProgressPrinter(int const barLength = 30)
		: Progress(-1), BarLength(barLength)
	{}

	void begin()
	{
		printf("Progress: \n");
		update(0);
	}

	void update(int const progress)
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

	void end()
	{
		update(100);
		printf("\n");
	}

};


#endif
