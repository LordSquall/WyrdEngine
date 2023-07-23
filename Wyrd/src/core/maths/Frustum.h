#pragma once

/* local includes */
#include "core/export.h"
#include "core/maths/Plane.h"
#include "core/maths/Ray.h"

/* external includes */
#include <glm/glm.hpp>

namespace Wyrd {

	struct WYRD_LIBRARY_API Frustum
	{
		Plane topFace;
		Plane bottomFace;

		Plane rightFace;
		Plane leftFace;

		Plane farFace;
		Plane nearFace;
	};
}