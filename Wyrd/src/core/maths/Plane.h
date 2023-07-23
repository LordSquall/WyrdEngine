#pragma once

/* local includes */
#include "core/export.h"
#include "core/maths/Rect.h"
#include "core/maths/Ray.h"

/* external includes */
#include <glm/glm.hpp>

namespace Wyrd {

	struct WYRD_LIBRARY_API Plane
	{
		glm::vec3 normal;
		float distance;

		Plane() = default;

		Plane(const glm::vec3& p1, const glm::vec3& norm) 
			: normal (glm::normalize(norm)), distance(glm::dot(normal, p1))
		{}

		float getSignedDistanceToPlane(const glm::vec3& point) const
		{
			return glm::dot(normal, point) - distance;
		}

		static inline glm::vec3 IntersectionPoint(const Plane a, const Plane b, const Plane c)
		{
			// Formula used
			//                d1 ( N2 * N3 ) + d2 ( N3 * N1 ) + d3 ( N1 * N2 )
			//P =   ---------------------------------------------------------------------
			//                             N1 . ( N2 * N3 )
			//
			// Note: N refers to the normal, d refers to the displacement. '.' means dot product. '*' means cross product

			glm::vec3 v1, v2, v3;
			float f = -glm::dot(a.normal, glm::cross(b.normal, c.normal));

			v1 = (a.distance * (glm::cross(b.normal, c.normal)));
			v2 = (b.distance * (glm::cross(c.normal, a.normal)));
			v3 = (c.distance * (glm::cross(a.normal, b.normal)));

			glm::vec3 vec = { v1.x + v2.x + v3.x, v1.y + v2.y + v3.y, v1.z + v2.z + v3.z };
			return vec / f;
		}
	};
}