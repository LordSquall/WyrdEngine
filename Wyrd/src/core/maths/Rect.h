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
			_position = { 0.0f, 0.0f };
			_size = { 0.0f, 0.0f };
		}

		Rect(const glm::vec2& pos, const glm::vec2& size)
		{
			_position = pos;
			_size = size;
		}

		bool const ContainsPoint(glm::vec2 r);
		bool const ContainsRect(Rect r);
		bool const ContainsRay(Ray r, Ray::Hit& hit);

		void Merge(Rect& r);

	public:
		glm::vec2 _position;
		glm::vec2 _size;

		std::array<Rect*, 4> contact;
	};

	/* comparison operators */
	inline bool operator==(const Rect& lhs, const Rect& rhs) { return (lhs._position == rhs._position) && (lhs._size == rhs._size); }
	inline bool operator!=(const Rect& lhs, const Rect& rhs) { return !operator==(lhs, rhs); }

	/* arithmetric operations */
	inline Rect operator*(glm::mat4 lhs, const Rect& rhs)
	{
		glm::vec4 result = lhs * glm::vec4({ rhs._position, rhs._size });

		Rect r;
		r._position = { result.x, result.y };
		r._size = { result.z, result.w };
		return r;
	}
}