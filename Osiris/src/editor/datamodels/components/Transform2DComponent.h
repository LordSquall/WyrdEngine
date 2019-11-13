#pragma once

#include <Osiris.h>
#include <glm/glm.hpp>

#include "editor/tools/PropertiesViewer/IPropertyComponent.h"

using namespace Osiris;
using namespace glm;

namespace Osiris::Editor {

	class Transform2DComponent : public IPropertiesComponent
	{
	public:
		Transform2DComponent();
		Transform2DComponent(const Transform2DComponent& obj);
		~Transform2DComponent();

		vec2 position;
		float rotation;
		vec2 scale;

		// Inherited via TransformComponent
		void OnPropertyEditorDraw() override;
	};
}