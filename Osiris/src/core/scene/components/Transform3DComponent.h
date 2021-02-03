#pragma once

/* local includes */
#include "core/scene/components/IBaseComponent.h"

/* external includes */
#include <glm/glm.hpp>

using namespace glm;

namespace Osiris {
	
	class GameObject;

	class OSR_LIBRARY_API Transform3DComponent : public IBaseComponent
	{
	public:
		Transform3DComponent(std::shared_ptr<GameObject> gameObject);
		Transform3DComponent(const Transform3DComponent& obj);
		~Transform3DComponent();

		inline virtual const std::string GetManagedType() { return "Transform3DComponent"; }

		vec3 position;
		vec3 rotation;
		vec3 scale;
	};
}