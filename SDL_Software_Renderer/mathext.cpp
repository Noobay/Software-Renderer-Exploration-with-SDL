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
	// Scale, bias and saturate x to 0..1 range
	value = clamp((value - edge0) / (edge1 - edge0), 0.0f, 1.0f);
	// Evaluate polynomial
	return value * value * (3 - 2 * value);
 }