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


	glm::vec2 Rect::GetDepth(const glm::vec4 rectA, const glm::vec4 rectB)
	{
		glm::vec2 centerA = { (rectA.x + (rectA.z * 0.5f)), (rectA.y + (rectA.w * 0.5f)) };
		glm::vec2 centerB = { (rectB.x + (rectB.z * 0.5f)), (rectB.y + (rectB.w * 0.5f)) };

		glm::vec2 maxDepth = { (rectA.z * 0.5f) + (rectB.z * 0.5f), (rectA.w * 0.5f) + (rectB.w * 0.5f) };

		return { (centerA.x + centerB.x) - maxDepth.x, (centerA.y - centerB.y) - maxDepth.y };
	}
}