#include "osrpch.h"
#include "MathHelper.h"

namespace Osiris {

	bool Rect::Contains(const glm::vec4 rect, glm::vec2 point)
	{
		if (point.x >= rect.x && point.x <= (rect.x + rect.z))
			if (point.y >= rect.y && point.y <= (rect.y + rect.w))
				return true;

		return false;
	}
}