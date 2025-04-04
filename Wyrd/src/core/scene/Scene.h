#pragma once

/* local includes */
#include "core/export.h"
#include "core/UID.h"
#include "core/Structures.h"
#include "core/ecs/ECS.h"
#include "core/ecs/ComponentPool.h"

/* external includes */
#include <glm/glm.hpp>

namespace Wyrd {

	class Layer2D;
	class Behaviour;

	/* Editor Scene Data Model Structure */
	class WYRD_LIBRARY_API Scene
	{
	public:
		/* Constructors */
		Scene();
		Scene(std::string name);
		~Scene() {}

		bool Initialise();

		void AssignScripts(const Entity entity);

		inline const Entity GetPrimaryCameraEntity() { return _ScenePrimaryCameraEntity; }
		inline void SetPrimaryCameraEntity(const Entity entity) { _ScenePrimaryCameraEntity = entity; }

		/**
		 * @brief Create a new entity within the scene with no components
		 * @return Entity
		*/
		Entity CreateEntity();

		/**
		 * @brief Create a new entity within the scene with all basic components
		 * @param name - name for the entity to pass to the metadata component
		 * @return Entity
		*/
		Entity CreateEntity(const std::string& name);

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


		void CleanUp();

		/**
		 * @brief Register a new component type with the ECS
		 * @param name Entity Name
		*/
		template<typename Component>
		void RegisterComponent(const std::string& name, const std::string& scriptName, bool serialise, ComponentResetFunc resetFunc)
		{
			int componentID = GetID<Component>();

			if (componentPools.size() <= componentID)
			{
				componentPools.resize(componentID + 1, nullptr);
			}

			componentPools[componentID] = new ComponentPool(name, componentID, sizeof(Component), scriptName, serialise, resetFunc);
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
			
			if (entity > componentPools[componentID]->count)
			{
				componentPools[componentID]->count++;
			}

			return comp;
		}

		/**
		 * @brief Initialise the base memory of a component object to a respective entity
		 * @param Entity to assigned to
		*/
		template<typename Component>
		void InitialiseComponent(Entity entity)
		{
			int componentID = GetID<Component>();
			(new (componentPools[componentID]->get(entity)) Component());
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
		void RemoveComponent(uint32_t poolIndex, Entity entity);

		/**
		 * @brief Retrieve a Component from a entity
		 * @param Entity to query
		 * @return Pointer to the component, nullptr if not assigned
		*/
		template<typename Component>
		Component* Get(Entity entity)
		{
			if (entity == ENTITY_INVALID)
				return nullptr;

			if (entities.size() <= entity - 1)
				return nullptr;

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
		void* Get(uint32_t poolIndex, Entity entity) const
		{
			if (entity == ENTITY_INVALID)
				return nullptr;

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
		
	public:
		std::string name;
		Color bgcolor;

		Wyrd::Vector3 cameraPosition = { 0.0f, 0.0f, 0.0f };
		Wyrd::Vector3 cameraOrientation = { 0.0f, 0.0f, 0.0f };
		float cameraZoom = 0.0f;

	private:
		Entity _ScenePrimaryCameraEntity;
		std::vector<Entity> _AvailableEntities;

	public:
		std::vector<EntityDesc> entities;
		std::vector<ComponentPool*> componentPools;
	};
}