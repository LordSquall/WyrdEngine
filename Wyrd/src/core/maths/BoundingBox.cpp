/* local includes */
#include "wyrdpch.h"
#include "BoundingBox.h"
#include "core/Log.h"

namespace Wyrd {

	bool const BoundingBox::ContainsPoint(glm::vec2 p)
	{
		return false;// (p.x >= _position.x && p.y >= _position.y && p.x < _position.x + _size.x && p.y < _position.y + _size.y);
	}

	bool const BoundingBox::ContainsBoundingBox(BoundingBox bb)
	{
		return false;// (_position.x < r._position.x + r._size.x && _position.x + _size.x > r._position.x) && (_position.y < r._position.y + r._size.y && _position.y + _size.y > r._position.y);
	}

	bool const BoundingBox::ContainsRay(Ray r, Ray::Hit& hit)
	{
		// Cache division
		glm::vec2 invdir = 1.0f / r.dir;

		//WYRD_TRACE("Ray: {0},{1},{2},{3}", r.origin.x, r.origin.y, invdir.x, invdir.y);

		// Calculate intersections with rectangle bounding axes
		//glm::vec2 t_near = (_position - r.origin) * invdir;
		//glm::vec2 t_far = (_position + {1.0f, 1.0f} - r.origin)* invdir;
		//
		//if (std::isnan(t_far.y) || std::isnan(t_far.x)) return false;
		//if (std::isnan(t_near.y) || std::isnan(t_near.x)) return false;
		//
		//// Sort distances
		//if (t_near.x > t_far.x) std::swap(t_near.x, t_far.x);
		//if (t_near.y > t_far.y) std::swap(t_near.y, t_far.y);
		//
		//// Early rejection		
		//if (t_near.x > t_far.y || t_near.y > t_far.x) return false;
		//
		//// Closest 'time' will be the first contact
		//hit.distance = std::max<float>(t_near.x, t_near.y);
		//
		//// Furthest 'time' is contact on opposite side of target
		//float t_hit_far = std::min<float>(t_far.x, t_far.y);
		//
		//// Reject if ray direction is pointing away from object
		//if (t_hit_far < 0)
		//	return false;
		//
		//// Contact point of collision from parametric line equation
		//hit.point = r.origin + hit.distance * r.dir;
		//
		//if (t_near.x > t_near.y)
		//	if (invdir.x < 0)
		//		hit.normal = { 1, 0 };
		//	else
		//		hit.normal = { -1, 0 };
		//else if (t_near.x < t_near.y)
		//	if (invdir.y < 0)
		//		hit.normal = { 0, 1 };
		//	else
		//		hit.normal = { 0, -1 };
		//
		// Note if t_near == t_far, collision is principly in a diagonal
		// so pointless to resolve. By returning a CN={0,0} even though its
		// considered a hit, the resolver wont change anything.
		return true;
	}
}