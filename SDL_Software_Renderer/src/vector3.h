#include "math.h"

struct Vector3
{
	float x;
	float y;
	float z;

	inline Vector3() 
	{
		x = 0.0f;
		y = 0.0f;
		z = 0.0f;
	}

	inline Vector3(const float &x_value, const float &y_value, const float &z_value)
	{
		x = x_value;
		y = y_value;
		z = z_value;
	}
};