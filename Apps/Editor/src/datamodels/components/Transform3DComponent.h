#pragma once

#include <Osiris.h>
#include <glm/glm.hpp>

#include "views/PropertiesViewer/IPropertyComponent.h"

using namespace Osiris;
using namespace glm;

namespace Osiris::Editor {
	
	class Transform3DComponent : public IPropertiesComponent
	{
	public:
		Transform3DComponent();
		Transform3DComponent(const Transform3DComponent& obj);
		~Transform3DComponent();

		vec3 position;
		vec3 rotation;
		vec3 scale;

		// Inherited via TransformComponent
		void OnPropertyEditorDraw() override;
	};
}