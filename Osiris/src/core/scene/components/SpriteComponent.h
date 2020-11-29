#pragma once

/* local includes */
#include "core/scene/components/IBaseComponent.h"

/* external includes */
#include <glm/glm.hpp>

using namespace glm;

namespace Osiris {

	class GameObject;
	class Shader;
	class Texture;
	class Sprite;

	class OSR_LIBRARY_API SpriteComponent : public IBaseComponent
	{
	public:
		SpriteComponent(std::shared_ptr<GameObject> owner);
		SpriteComponent(const SpriteComponent& obj);
		~SpriteComponent();

		std::shared_ptr<Shader> shader;
		std::shared_ptr<Texture> BaseTexture;
		std::shared_ptr<Sprite> sprite;
		vec4 Color;

	public:
		void Initialise();
		void Recalculate();
		void Render(Timestep ts, Renderer& renderer);

		std::string toString() const { return "Hello"; }
	};
}