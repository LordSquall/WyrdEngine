#pragma once

/* local includes */
#include "Osiris.h"
#include "core/scene/components/IBaseComponent.h"
#include "core/renderer/Texture.h"
#include "core/renderer/Shader.h"
#include "core/pipeline/Sprite.h"

/* external includes */
#include <glm/glm.hpp>

using namespace glm;

namespace Osiris {

	class GameObject;

	class OSR_LIBRARY_API SpriteComponent : public IBaseComponent
	{
	public:
		SpriteComponent(std::shared_ptr<GameObject> owner);
		SpriteComponent(const SpriteComponent& obj);
		~SpriteComponent();

		std::shared_ptr<Shader> Shader;
		std::shared_ptr<Texture> BaseTexture;
		std::shared_ptr<Sprite> Sprite;
		vec4 Color;

	public:
		void Recalculate();
		void Render(Timestep ts, Renderer& renderer);

		std::string toString() const { return "Hello"; }
	};
}