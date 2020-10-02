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

	bool Rect::ContainsRay(Ray r, Ray::Hit& hit)
	{
		// Cache division
		glm::vec2 invdir = 1.0f / r.dir;

		// Calculate intersections with rectangle bounding axes
		glm::vec2 t_near = (position - r.origin) * invdir;
		glm::vec2 t_far = (position + size - r.origin) * invdir;

		if (std::isnan(t_far.y) || std::isnan(t_far.x)) return false;
		if (std::isnan(t_near.y) || std::isnan(t_near.x)) return false;

		// Sort distances
		if (t_near.x > t_far.x) std::swap(t_near.x, t_far.x);
		if (t_near.y > t_far.y) std::swap(t_near.y, t_far.y);

		// Early rejection		
		if (t_near.x > t_far.y || t_near.y > t_far.x) return false;

		// Closest 'time' will be the first contact
		hit.distance = std::max(t_near.x, t_near.y);

		// Furthest 'time' is contact on opposite side of target
		float t_hit_far = std::min(t_far.x, t_far.y);

		// Reject if ray direction is pointing away from object
		if (t_hit_far < 0)
			return false;

		// Contact point of collision from parametric line equation
		hit.point = r.origin + hit.distance * r.dir;

		if (t_near.x > t_near.y)
			if (invdir.x < 0)
				hit.normal = { 1, 0 };
			else
				hit.normal = { -1, 0 };
		else if (t_near.x < t_near.y)
			if (invdir.y < 0)
				hit.normal = { 0, 1 };
			else
				hit.normal = { 0, -1 };

		// Note if t_near == t_far, collision is principly in a diagonal
		// so pointless to resolve. By returning a CN={0,0} even though its
		// considered a hit, the resolver wont change anything.
		return true;
	}
}