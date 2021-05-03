#pragma once

/* local includes */
#include "core/export.h"

/* external includes */
#include <glm/glm.hpp>

namespace Wyrd {

	class WYRD_LIBRARY_API Ray
	{
	public:
		struct Hit
		{
			glm::vec2 point;
			glm::vec2 normal;
			float distance;
		};

	public:
		glm::vec2 origin;
		glm::vec2 dir;
	};
}