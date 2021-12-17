#include "mathext.h"


float clamp(float value, float minimum, float maximum)
{
	float clampedValue = value;

	clampedValue = std::min(maximum, clampedValue);
	clampedValue = std::max(minimum, clampedValue);

	return clampedValue;
}

float smoothstep(float edge0, float edge1, float value) 
{
	value = clamp((value - edge0) / (edge1 - edge0), 0.0f, 1.0f);
	return value * value * (3 - 2 * value);
 }