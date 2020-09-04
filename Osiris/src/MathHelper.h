#pragma once

/* local includes */
#include "core/export.h"
#include "core/physics/Polygon.h"

/* external includes */
#include <glm/glm.hpp>

namespace Osiris {

	class OSR_LIBRARY_API Rect
	{
	public:
		static bool Contains(const glm::vec4 rect, const glm::vec2 point);
		static bool Contains(const glm::vec4 rectA, const glm::vec4 rectB);

		static glm::vec2 GetDepth(const glm::vec4 rectA, const glm::vec4 rectB);
	};
}