#pragma once

/* local includes */
#include "core/scene/components/IBaseComponent.h"
#include "core/Structures.h"

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
		~SpriteComponent() = default;

		inline virtual const std::string GetManagedType() { return "SpriteComponent"; }

		std::shared_ptr<Shader> shader;
		std::shared_ptr<Texture> texture;
		glm::vec2 position;
		glm::vec2 size;
		Color color;

	public:
		void Initialise();
		void Recalculate();
		void Render(Timestep ts, Renderer& renderer);

		std::string toString() const { return "Hello"; }
	};
}