#pragma once

/* Local includes */
#include "core/export.h"
#include "core/scene/components/IBaseComponent.h"
#include "core/UID.h"

/* external includes */
#include <glm/glm.hpp>

namespace Osiris {

	class Behaviour;
	class Layer2D;
	class Transform2DComponent;
	class SpriteComponent;
	class ScriptComponent;
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

		std::shared_ptr<GameObject>* AddChild(std::shared_ptr<GameObject> gameObject);
		void RemoveChild(UID uid);
		void RemoveChildren();
		void DuplicateChild(UID uid);
		void SwapChild(int a, int b);

		void AddComponent(std::shared_ptr<IBaseComponent> component);

		std::shared_ptr<GameObject> FindChild(const UID uid);
		std::shared_ptr<ScriptComponent> FindScriptComponent(const std::string& name);

		void AssignScripts(Behaviour* behaviour);
	public:
		std::string name;

		std::shared_ptr<Transform2DComponent> transform2D;
		std::shared_ptr<SpriteComponent> sprite;
		std::shared_ptr<PhysicsComponent> physics;

		std::vector<std::shared_ptr<IBaseComponent>> components;
		
		std::shared_ptr<Layer2D> layer;

		std::shared_ptr<GameObject> parent;
		std::vector<std::shared_ptr<GameObject>> children;

		glm::vec4 inputArea;

		UID uid;
	};

}