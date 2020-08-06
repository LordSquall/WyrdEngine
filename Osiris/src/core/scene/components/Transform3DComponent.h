#pragma once

/* local includes */
#include "Osiris.h"
#include "core/scene/components/IBaseComponent.h"

/* external includes */
#include <glm/glm.hpp>

using namespace glm;

namespace Osiris {
	
	class OSR_LIBRARY_API Transform3DComponent : public IBaseComponent
	{
	public:
		Transform3DComponent(std::shared_ptr<GameObject> gameObject);
		Transform3DComponent(const Transform3DComponent& obj);
		~Transform3DComponent();

		vec3 position;
		vec3 rotation;
		vec3 scale;
	};
}