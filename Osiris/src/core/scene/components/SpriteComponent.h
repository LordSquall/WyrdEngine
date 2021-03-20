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
	class Layer2D;

	class OSR_LIBRARY_API SpriteComponent : public IBaseComponent
	{
	public:
		SpriteComponent(GameObject* gameObject);
		SpriteComponent(const SpriteComponent& obj);
		~SpriteComponent() = default;

		inline virtual const std::string GetManagedType() { return "SpriteComponent"; }

		void SetSize(float width, float height);
		void SetPosition(float x, float y);

		void SetTexture(std::shared_ptr<Texture> texture);
		const std::shared_ptr<Texture> GetTexture();

		std::shared_ptr<Shader> shader;
		std::shared_ptr<Layer2D> spriteLayer;
		glm::vec2 position;
		glm::vec2 size;
		Color color;
		int32_t BatchIndex;

	public:
		void Initialise();
		void Recalculate();
		void Render(Timestep ts, Renderer& renderer);
		void Remove();

		std::string toString() const { return "Hello"; }

	private:
		std::shared_ptr<Texture> _Texture;
	};
}