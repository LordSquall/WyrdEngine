#pragma once

#include "core/export.h"

#include <glm/glm.hpp>

namespace Osiris {

	class Rect
	{
	public:
		static bool Contains(const glm::vec4 rect, const glm::vec2 point);
	};
}