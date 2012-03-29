#ifndef _CABBAGE_CORE_UTILS_H_INCLUDED_
#define _CABBAGE_CORE_UTILS_H_INCLUDED_

#include <cmath>
#include <iostream>

const float RoundingError32 = 0.00001f;

static bool const equals(float const a, float const b, float const epsilon = RoundingError32)
{
	return (a + epsilon >= b) && (a - epsilon <= b);
}

// Portable version of system("PAUSE")
static void waitForUser() 
{
	std::cout << "Press [Enter] to continue . . .";
	std::cin.get();
}

static float frand()
{
	return std::rand() / (float) RAND_MAX;
}

#endif
