#pragma once

/* Local includes */
#include "core/export.h"
#include "core/UID.h"

/* external includes */
#include <glm/glm.hpp>
#include <jsonxx.h>

namespace Osiris {

	class Behaviour;
	class Layer2D;
	class IBaseComponent;
	class TransformComponent;
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
		GameObject(std::string name, bool generateUID = false);
		~GameObject();

		GameObject* AddChild(std::unique_ptr<GameObject> gameObject);
		void RemoveChild(UID uid);
		void RemoveChildren();
		void DuplicateChild(UID uid);
		void SwapChild(int a, int b);

		void AddComponent(std::unique_ptr<IBaseComponent> component);

		std::shared_ptr<GameObject> FindChild(const UID uid);
		ScriptComponent* FindScriptComponent(const std::string& name);

		void AssignScripts(Behaviour* behaviour);

		inline std::vector<std::unique_ptr<GameObject>>& GetGameObjects() { return _GameObjects; }

		/**
		 * @brief Serialise the gameobject into a json object
		 * @return json object
		*/
		jsonxx::Object ToJson();

		/**
		 * @brief Deserialise a json object into a gameobject
		 * @param object json object
		 * @return true is successful
		*/
		bool FromJson(jsonxx::Object& object);

	public:
		std::string name;

		std::unique_ptr<TransformComponent> transform;

		std::shared_ptr<Transform2DComponent> transform2D;
		std::shared_ptr<SpriteComponent> sprite;
		std::shared_ptr<PhysicsComponent> physics;

		std::vector<std::unique_ptr<IBaseComponent>> components;
		
		std::shared_ptr<Layer2D> layer;

		std::shared_ptr<GameObject> parent;
		std::vector<std::shared_ptr<GameObject>> children;

		glm::vec4 inputArea;

		UID uid;

	protected:
		std::vector<std::unique_ptr<GameObject>> _GameObjects;
	};

}