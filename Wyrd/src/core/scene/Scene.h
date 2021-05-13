#pragma once

/* local includes */
#include "core/export.h"
#include "core/UID.h"
#include "core/Structures.h"
#include "core/scene/SceneLayer.h"
#include "core/ecs/ECS.h"
#include "core/ecs/ComponentPool.h"

#include <glm/glm.hpp>

namespace Wyrd {

	class Layer2D;
	class GameObject;
	class Behaviour;
	class CameraComponent;

	/* Editor Scene Data Model Structure */
	class WYRD_LIBRARY_API Scene
	{
	public:
		/* Constructors */
		Scene();
		Scene(std::string name);
		~Scene() {}

		void Update();

		inline std::vector<std::unique_ptr<SceneLayer>>& GetLayers() { return _Layers; }

		GameObject* FindGameObject(const UID uid);

		void AssignScripts(Behaviour* behaviour);

		const std::unique_ptr<SceneLayer>& AddLayer(std::unique_ptr<SceneLayer> layer);
		void RemoveLayer(const UID& uid);


		inline const UID& GetPrimaryCameraUID() { return _ScenePrimaryCamera; }
		inline void SetPrimaryCameraUID(const UID& uid) { _ScenePrimaryCamera = uid; }

		CameraComponent* GetPrimaryCamera();

		/**
		 * @brief Create a new entity within the scene
		 * @return Entity
		*/
		Entity CreateEntity();

		/**
		 * @brief Assign a component object to a respective entity
		 * @param Entity to assigned to
		 * @return Pointer to a newly allocated component
		*/
		template<typename Component>
		Component* AssignComponent(Entity entity)
		{
			int componentID = GetID<Component>();

			// ensure there enough space
			if (componentPools.size() <= componentID)
			{
				componentPools.resize(componentID + 1, nullptr);
			}

			// create if not initialied
			if (componentPools[componentID] == nullptr)
			{
				componentPools[componentID] = new ComponentPool(sizeof(Component));
			}

			Component* comp = new (componentPools[componentID]->get(entity)) Component();

			entities[entity].mask.set(componentID);
			return comp;
		}

		/**
		 * @brief Retrieve a Component from a entity
		 * @param Entity to query
		 * @return Pointer to the component, nullptr if not assigned
		*/
		template<typename Component>
		Component* Get(Entity entity)
		{
			int componentID = GetID<Component>();
			if (!entities[entity].mask.test(componentID))
				return nullptr;

			Component* comp = static_cast<Component*>(componentPools[componentID]->get(entity));
			return comp;
		}

		/**
		 * @brief Serialise the scene into a json object
		 * @return json object
		*/
		jsonxx::Object ToJson();

		/**
		 * @brief Deserialise a json object into a scene
		 * @param object json object
		 * @return true is successful
		*/
		bool FromJson(jsonxx::Object& object);

	public:
		std::string name;
		Color bgcolor;

		glm::vec3 cameraPosition = { 0.0f, 0.0f, 0.0f };
		float cameraZoom = 0.0f;

	private:
		std::vector<std::unique_ptr<SceneLayer>> _Layers;
		UID _ScenePrimaryCamera;

	public:
		std::vector<EntityDesc> entities;
		std::vector<ComponentPool*> componentPools;
	};
}