#pragma once

/* Local includes */
#include "core/export.h"
#include "core/scene/components/IBaseComponent.h"

/* external includes */
#include <glm/glm.hpp>

namespace Osiris {

	class Layer2D;
	class Transform2DComponent;
	class SpriteComponent;
	class PhysicsComponent;

	/* Editor GameObject Data Model Structure */
	class OSR_LIBRARY_API GameObject
	{
	public:
		/* Constructors */
		GameObject();
		GameObject(std::string name);
		GameObject(const GameObject& obj);
		~GameObject();

		inline uint32_t GetUID() { return _uid; }

	public:
		std::string name;

		std::shared_ptr<Transform2DComponent> transform2D;
		std::shared_ptr<SpriteComponent> sprite;
		std::shared_ptr<PhysicsComponent> physics;

		std::vector<std::shared_ptr<IBaseComponent>> components;
		
		std::shared_ptr<Layer2D> layer;

		std::vector<std::shared_ptr<GameObject>> children;

		glm::vec4 inputArea;
	private:
		uint32_t _uid = 0u;
		static uint32_t _nextUid;

	};

}