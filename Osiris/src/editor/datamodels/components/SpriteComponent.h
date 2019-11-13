#pragma once

#include <Osiris.h>
#include <glm/glm.hpp>

#include "editor/tools/PropertiesViewer/IPropertyComponent.h"
#include "editor/datamodels/resources/TextureRes.h"
#include "core/pipeline/Sprite.h"

using namespace Osiris;
using namespace glm;

namespace Osiris::Editor {

	class SpriteComponent : public IPropertiesComponent
	{
	public:
		SpriteComponent();
		SpriteComponent(const SpriteComponent& obj);
		~SpriteComponent();

		std::shared_ptr<TextureRes> BaseTexture;
		std::shared_ptr<Sprite> Sprite;

		// Inherited via TransformComponent
		void OnPropertyEditorDraw() override;
	};
}