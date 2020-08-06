#include "osrpch.h"
#include "MathHelper.h"

namespace Osiris {

	static bool valueInRange(int value, int min, int max)
	{
		return (value >= min) && (value <= max);
	}

	static bool valueInRange(float value, float min, float max)
	{
		return (value >= min) && (value <= max);
	}

	bool Rect::Contains(const glm::vec4 rect, glm::vec2 point)
	{
		if (point.x >= rect.x && point.x <= (rect.x + rect.z))
			if (point.y >= rect.y && point.y <= (rect.y + rect.w))
				return true;

		return false;
	}

	bool Rect::Contains(const glm::vec4 rectA, glm::vec4 rectB)
	{
		bool xOverlap = valueInRange(rectA.x, rectB.x, rectB.x + rectB.z) ||
			valueInRange(rectB.x, rectA.x, rectA.x + rectA.z);

		bool yOverlap = valueInRange(rectA.y, rectB.y, rectB.y + rectB.w) ||
			valueInRange(rectB.y, rectA.y, rectA.y + rectA.w);

		return xOverlap && yOverlap;
	}

}