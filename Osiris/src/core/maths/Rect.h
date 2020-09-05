#pragma once

/* local includes */
#include "core/export.h"

/* external includes */
#include <glm/glm.hpp>

namespace Osiris {

	class OSR_LIBRARY_API Rect
	{
	public:
		bool ContainsPoint(glm::vec2 point);
		bool ContainsRect(Rect rect);



	public:
		glm::vec2 position;
		glm::vec2 size;
	};

	/* comparison operators for Rect */
	inline bool operator==(const Rect& lhs, const Rect& rhs) { return (lhs.position == rhs.position) && (lhs.size == rhs.size); }
	inline bool operator!=(const Rect& lhs, const Rect& rhs) { return !operator==(lhs, rhs); }

	inline Rect operator*(glm::mat4 lhs, const Rect& rhs)
	{
		glm::vec4 result = lhs * glm::vec4({ rhs.position, rhs.size });

		return { { result.x, result.y }, {result.z, result.w } };
	}
}