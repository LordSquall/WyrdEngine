#pragma once

/* core osiris include */
#include <core/scene/components/SpriteComponent.h>

/* local includes */
#include "views/PropertiesViewer/IPropertyView.h"

/* external includes */
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>

using namespace glm;

namespace Osiris::Editor {

	class SpriteComponentView : public IPropertiesView
	{
	public:
		SpriteComponentView(Osiris::SpriteComponent* baseComponent) : _BaseComponent(baseComponent) {}
		SpriteComponentView(const SpriteComponentView& obj) {}

		void OnPropertyEditorDraw() override;

	private:
		Osiris::SpriteComponent* _BaseComponent;
	};
}