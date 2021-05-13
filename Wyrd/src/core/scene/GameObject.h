#pragma once

/* Local includes */
#include "core/export.h"
#include "core/UID.h"

/* external includes */
#include <glm/glm.hpp>

namespace jsonxx
{
	class Object;
};

namespace Wyrd {

	class Behaviour;
	class SceneLayer;
	class IBaseComponent;
	class TransformComponent;
	class Transform2DComponent;
	class SpriteComponent;
	class ScriptComponent;
	class PhysicsComponent;
	class CameraComponent;

	/* Editor GameObject Data Model Structure */
	class WYRD_LIBRARY_API GameObject
	{
	public:
		/* Constructors */
		GameObject();
		GameObject(std::string name, bool generateUID = false);
		~GameObject();

		void RemoveChild(UID uid);
		void RemoveChildren();
		void DuplicateChild(UID uid);
		void SwapChild(int a, int b);

		IBaseComponent* AddComponent(std::unique_ptr<IBaseComponent> component);

		GameObject* FindChild(const UID uid);
		ScriptComponent* FindScriptComponent(const std::string& name);
		CameraComponent* FindCameraComponent(const UID& uid);

		virtual void AssignScripts(Behaviour* behaviour);

		inline std::vector<std::unique_ptr<GameObject>>& GetGameObjects() { return _GameObjects; }

		/**
		 * Set the layer which this game object belongs too
		*/
		inline void SetSceneLayer(SceneLayer* sceneLayer) { _SceneLayer = sceneLayer; }

		/**
		 * Get the layer which this game object belongs too
		*/
		inline SceneLayer* GetSceneLayer() const { return _SceneLayer; }

		/**
		 * @brief Serialise the gameobject into a json object
		 * @return json object
		*/
		bool ToJson(jsonxx::Object& object);

		/**
		 * @brief Deserialise a json object into a gameobject
		 * @param object json object
		 * @return true is successful
		*/
		bool FromJson(jsonxx::Object& object);

	public:
		std::string name;

		std::unique_ptr<TransformComponent> transform;

		std::shared_ptr<SpriteComponent> sprite;
		std::shared_ptr<PhysicsComponent> physics;

		std::vector<std::unique_ptr<IBaseComponent>> components;
	
		glm::vec4 inputArea;

		UID uid;

	protected:
		SceneLayer* _SceneLayer;

		std::vector<std::unique_ptr<GameObject>> _GameObjects;
	};

}