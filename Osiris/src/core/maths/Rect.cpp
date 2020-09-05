/* local includes */
#include "osrpch.h"
#include "Rect.h"

namespace Osiris {

	bool Rect::ContainsPoint(glm::vec2 point)
	{
		return true;
	}

	bool Rect::ContainsRect(Rect rect)
	{
		return true;
	}
}