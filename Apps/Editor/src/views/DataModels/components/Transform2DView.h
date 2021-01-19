#pragma once

/* core osiris include */
#include <core/scene/components/Transform2DComponent.h>

/* local includes */
#include "views/PropertiesViewer/IPropertyView.h"

/* external includes */
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>

using namespace Osiris;
using namespace glm;

namespace Osiris::Editor {

	class Transform2DView : public IPropertiesView
	{
	public:
		Transform2DView(Transform2DComponent* baseComponent) : _BaseComponent(baseComponent) {}
		Transform2DView(const Transform2DView& obj) {}
	
		void OnPropertyEditorDraw() override;

		const std::string GetName() override { return "Transform (2D)"; }

	private:
		Transform2DComponent* _BaseComponent;
	};
}