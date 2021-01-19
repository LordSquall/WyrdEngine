#pragma once

/* core osiris include */
#include <core/scene/components/PhysicsComponent.h>

/* local includes */
#include "views/PropertiesViewer/IPropertyView.h"

/* external includes */
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>

using namespace Osiris;
using namespace glm;

namespace Osiris::Editor {

	class PhysicsComponentView : public IPropertiesView
	{
	public:
		PhysicsComponentView(PhysicsComponent* baseComponent) : _BaseComponent(baseComponent) {}
		PhysicsComponentView(const PhysicsComponentView& obj) {}

		void OnPropertyEditorDraw() override;
		void OnSceneViewerDraw(const glm::mat4& viewProjectMatrix) override;

		const std::string GetName() override { return "Physics"; }

	private:
		PhysicsComponent* _BaseComponent;
	};
}