#pragma once
#include "Vec3.h"

struct Ray
{
	Ray() = default;

	Ray(const Vec3<float>& origin, const Vec3<float>& direction) :
		m_origin{ origin }, m_direction{ direction }
	{

	}

	Vec3<float> pointAt(float t)
	{
		return m_origin + t * m_direction;
	}

public:
	Vec3<float> m_origin;
	Vec3<float> m_direction;
};

