#pragma once

/* local includes */
#include "core/export.h"
#include "core/UID.h"
#include "core/Structures.h"
#include "core/maths/BoundingBox.h"
#include "core/pipeline/Vertex3D.h"
#include "core/renderer/ResourceBase.h"

/* external includes */
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
#include <string>
#include <vector>

namespace Wyrd
{
	struct MeshDesc
	{
		ResourceDesc resource;
		std::vector<Vertex3D> vertices;
		BoundingBox boundingBox;

		MeshDesc() : resource() { }
	};

	class WYRD_LIBRARY_API Mesh : public ResourceBase
	{
	public:
		std::vector<Vertex3D> Vertices;
		BoundingBox boundingBox;
		UID BaseTexture;


		static Mesh* Create(const MeshDesc& desc);
	};
}