#pragma once

/* local includes */
#include "core/export.h"
#include "core/UID.h"
#include "core/Structures.h"
#include "core/ecs/ECS.h"
#include "core/ecs/ComponentPool.h"

/* external includes */
#include <glm/glm.hpp>

namespace jsonxx
{
	class Object;
}

namespace Wyrd {

	class Layer2D;
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

		bool Initialise();

		void AssignScripts(Behaviour* behaviour);

		inline const UID& GetPrimaryCameraUID() { return _ScenePrimaryCamera; }
		inline void SetPrimaryCameraUID(const UID& uid) { _ScenePrimaryCamera = uid; }

		/**
		 * @brief Create a new entity within the scene
		 * @return Entity
		*/
		Entity CreateEntity();

		/**
		 * @brief Create a new entity within the scene
		 * @param entity Entity to be destroyed
		 * @return Entity
		*/
		void DestroyEntity(Entity entity);

		/**
		 * @brief Swap entities 
		 * @param entityA Entity src
		 * @param entityB Entity dest
		 * @return Entity
		*/
		void SwapEntity(Entity entityA, Entity entityB);

		/**
		 * @brief Register a new component type with the ECS
		 * @param name Entity Name
		*/
		template<typename Component>
		void RegisterComponent(const std::string& name, const std::string& scriptName)
		{
			int componentID = GetID<Component>();

			if (componentPools.size() <= componentID)
			{
				componentPools.resize(componentID + 1, nullptr);
			}

			componentPools[componentID] = new ComponentPool(name, componentID, sizeof(Component), scriptName);
		}

		/**
		 * @brief Assign a component object to a respective entity
		 * @param Entity to assigned to
		 * @return Pointer to a newly allocated component
		*/
		template<typename Component>
		Component* AssignComponent(Entity entity)
		{
			int componentID = GetID<Component>();

			Component* comp = new (componentPools[componentID]->get(entity)) Component();

			entities[entity-1].mask.set(componentID);
			
			componentPools[componentID]->count++;

			return comp;
		}

		/**
		 * @brief Remove a component object to a respective entity
		 * @param Entity to remove from
		*/
		template<typename Component>
		void RemoveComponent(Entity entity)
		{
			int componentID = GetID<Component>();

			entities[entity - 1].mask.set(componentID, false);

			componentPools[componentID]->count--;
		}

		/**
		 * @brief Remove a component object to a respective entity
		 * @param Entity to remove from
		*/
		void RemoveComponent(uint32_t poolIndex, Entity entity)
		{
			entities[entity - 1].mask.set(poolIndex, false);

			componentPools[poolIndex]->count--;
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
			if (!entities[entity-1].mask.test(componentID))
				return nullptr;

			Component* comp = static_cast<Component*>(componentPools[componentID]->get(entity));
			return comp;
		}

		/**
		 * @brief Retrieve a Component from a entity based on a pool index
		 * @param Entity to query
		 * @return Pointer to the component, nullptr if not assigned
		*/
		void* Get(uint32_t poolIndex, Entity entity)
		{
			if (!entities[entity - 1].mask.test(poolIndex))
				return nullptr;

			return componentPools[poolIndex]->get(entity);
		}

		/**
		 * @brief Retrieve a Component Mask from a entity
		 * @param Entity to query
		 * @return Component Mask
		*/
		std::bitset<64> GetMask(Entity entity)
		{
			return entities[entity - 1].mask;
		}

		/**
		 * @brief Serialise the scene into a json object
		 * @param object json object
		 * @return true is successful
		*/
		bool ToJson(jsonxx::Object& object);

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
		UID _ScenePrimaryCamera;

	public:
		std::vector<EntityDesc> entities;
		std::vector<ComponentPool*> componentPools;
	};
}