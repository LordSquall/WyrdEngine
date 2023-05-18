#pragma once

/* local includes */
#include "core/export.h"
#include "core/maths/Ray.h"

/* external includes */
#include <glm/glm.hpp>

namespace Wyrd {

	class BoundingBox;

	class WYRD_LIBRARY_API BoundingBox
	{
	public:
		BoundingBox()
		{
			_position = { 0.0f, 0.0f, 0.0f };
			_minExtent = { -1.0f, -1.0f, -1.0f };
			_maxExtent = { 1.0f, 1.0f, 1.0f };
		}

		BoundingBox(const glm::vec3& pos)
		{
			_position = pos;
		}

		BoundingBox(float x, float y, float z)
		{
			_position.x = x;
			_position.y = y;
			_position.z = z;
		}

		bool const ContainsPoint(glm::vec2 r);
		bool const ContainsBoundingBox(BoundingBox r);
		bool const ContainsRay(Ray r, Ray::Hit& hit);

		void Merge(BoundingBox& r);

	public:
		glm::vec3 _position;
		glm::vec3 _minExtent;
		glm::vec3 _maxExtent;
	};

	/* comparison operators */
	inline bool operator==(const BoundingBox& lhs, const BoundingBox& rhs) { return (lhs._position == rhs._position); }
	inline bool operator!=(const BoundingBox& lhs, const BoundingBox& rhs) { return !operator==(lhs, rhs); }

	/* arithmetric operations */
	inline BoundingBox operator*(glm::mat4 lhs, const BoundingBox& rhs)
	{
		//glm::vec4 result = lhs * glm::vec4({ rhs._position, rhs._size });

		BoundingBox bb;
		//r._position = { result.x, result.y };
		//r._size = { result.z, result.w };
		return bb;
	}
}