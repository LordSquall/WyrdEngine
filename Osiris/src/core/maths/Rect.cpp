/* local includes */
#include "osrpch.h"
#include "Rect.h"

namespace Osiris {

	bool Rect::ContainsPoint(glm::vec2 p)
	{
		return (p.x >= position.x && p.y >= position.y && p.x < position.x + size.x && p.y < position.y + size.y);
	}

	bool Rect::ContainsRect(Rect r)
	{
		return (position.x < r.position.x + r.size.x && position.x + size.x > r.position.x) &&
			(position.y < r.position.y + r.size.y && position.y + size.y > r.position.y);
	}
}