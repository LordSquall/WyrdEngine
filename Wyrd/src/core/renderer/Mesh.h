#pragma once

/* local includes */
#include "core/export.h"
#include "core/Structures.h"
#include "core/pipeline/Vertex3D.h"

/* external includes */
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
#include <string>
#include <vector>

namespace Wyrd
{
	class WYRD_LIBRARY_API Mesh
	{
	public:
		std::vector<Vertex3D> Vertices;
		UID BaseTexture;
	};
}