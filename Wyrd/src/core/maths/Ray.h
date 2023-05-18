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
			glm::vec3 point;
			glm::vec3 normal;
			float distance;
		};

	public:
		glm::vec3 origin;
		glm::vec3 dir;
	};
}