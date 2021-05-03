#pragma once

/* core wyrd include */
#include <core/scene/components/SpriteComponent.h>

/* local includes */
#include "views/PropertiesViewer/IPropertyView.h"

/* external includes */
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>

using namespace glm;

namespace Wyrd::Editor {

	class SpriteComponentView : public IPropertiesView
	{
	public:
		SpriteComponentView(Wyrd::SpriteComponent* baseComponent) : _BaseComponent(baseComponent) {}
		SpriteComponentView(const SpriteComponentView& obj) {}

		void OnPropertyEditorDraw() override;

		const std::string GetName() override { return "Sprite"; }

	private:
		Wyrd::SpriteComponent* _BaseComponent;
	};
}