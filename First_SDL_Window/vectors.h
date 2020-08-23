struct Vector2
{
	float x;
	float y;

	static float Distance(const Vector2 &pointA, const Vector2 &pointB) {
		return sqrt(pow(pointA.x - pointB.x, 2.0f) + pow(pointA.y - pointB.y, 2.0f));
	}

	inline Vector2(const float &x_value, const float &y_value)
	{
		x = x_value;
		y = y_value;
	}

	inline Vector2 operator + (const Vector2 &other) const
	{
		return Vector2(x + other.x, y + other.y);
	}

	inline Vector2 operator - (const Vector2 &other) const
	{
		return Vector2(x - other.x, y - other.y);
	}

	inline Vector2 operator + (const float other) const 
	{
		return Vector2(x + other, y + other);
	}

	inline Vector2 operator - (const float other) const
	{
		return Vector2(x - other, y - other);
	}

	inline Vector2 operator * (const float other) const
	{
		return Vector2(x * other, y * other);
	}
};