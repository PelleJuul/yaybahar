#include "utils.h"
#include <cmath>

float map(float x, float oldMin, float oldMax, float newMin, float newMax)
{
    float r = (x - oldMin) / (oldMax - oldMin);
	return newMin + r * (newMax - newMin);
}

float pitchToFreq(float pitch)
{
    return 440 * std::pow(2, (pitch - 69) / 12);
}

float freqToPitch(float freq)
{
    return 12 * log2(freq / 440) + 69;
}