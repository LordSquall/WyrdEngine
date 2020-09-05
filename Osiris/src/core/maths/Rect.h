#pragma once

/* local includes */
#include "core/export.h"

/* external includes */
#include <glm/glm.hpp>

namespace Osiris {

	class OSR_LIBRARY_API Rect
	{
	public:
		bool ContainsPoint(glm::vec2 r);
		bool ContainsRect(Rect r);

	public:
		glm::vec2 position;
		glm::vec2 size;
	};

	/* comparison operators */
	inline bool operator==(const Rect& lhs, const Rect& rhs) { return (lhs.position == rhs.position) && (lhs.size == rhs.size); }
	inline bool operator!=(const Rect& lhs, const Rect& rhs) { return !operator==(lhs, rhs); }

	/* arithmetric operations */
	inline Rect operator*(glm::mat4 lhs, const Rect& rhs)
	{
		glm::vec4 result = lhs * glm::vec4({ rhs.position, rhs.size });

		return { { result.x, result.y }, {result.z, result.w } };
	}
}