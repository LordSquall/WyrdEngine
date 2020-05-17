#pragma once

#include <Osiris.h>
#include <glm/glm.hpp>

#include "tools/PropertiesViewer/IPropertyComponent.h"
#include "datamodels/resources/TextureRes.h"
#include "core/pipeline/Sprite.h"

using namespace Osiris;
using namespace glm;

namespace Osiris::Editor {

	class GameObject;

	class SpriteComponent : public IPropertiesComponent
	{
	public:
		SpriteComponent();
		SpriteComponent(const SpriteComponent& obj);
		~SpriteComponent();

		std::shared_ptr<GameObject> OwnerGameObject;
		std::shared_ptr<TextureRes> BaseTexture;
		std::shared_ptr<Sprite> Sprite;
		vec3 Color;

		// Inherited via TransformComponent
		void OnPropertyEditorDraw() override;
	};
}