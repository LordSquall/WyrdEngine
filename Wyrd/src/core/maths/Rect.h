#pragma once

/* local includes */
#include "core/export.h"
#include "core/maths/Ray.h"

/* external includes */
#include <glm/glm.hpp>

namespace Wyrd {

	class Rect;

	class WYRD_LIBRARY_API Rect
	{
	public:
		Rect()
		{
			position = { 0.0f, 0.0f };
			size = { 0.0f, 0.0f };
		}

		Rect(glm::vec2& pos, glm::vec2& size)
		{
			position = pos;
			size = size;
		}

		bool ContainsPoint(glm::vec2 r);
		bool ContainsRect(Rect r);
		bool ContainsRay(Ray r, Ray::Hit& hit);

		void Merge(Rect& r);

	public:
		glm::vec2 position;
		glm::vec2 size;

		std::array<Rect*, 4> contact;
	};

	/* comparison operators */
	inline bool operator==(const Rect& lhs, const Rect& rhs) { return (lhs.position == rhs.position) && (lhs.size == rhs.size); }
	inline bool operator!=(const Rect& lhs, const Rect& rhs) { return !operator==(lhs, rhs); }

	/* arithmetric operations */
	inline Rect operator*(glm::mat4 lhs, const Rect& rhs)
	{
		glm::vec4 result = lhs * glm::vec4({ rhs.position, rhs.size });

		Rect r;
		r.position = { result.x, result.y };
		r.position = { result.z, result.w };
		return r;
	}
}