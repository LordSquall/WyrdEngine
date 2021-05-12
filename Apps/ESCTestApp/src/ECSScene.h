#pragma once

#include "wyrdpch.h"

#include <core/ecs/Entity.h>

#include "ComponentPool.h"

using namespace Wyrd;

class ECSScene
{
public:
	ECSScene() {}


	Entity CreateEntity()
	{
		entities.push_back({ entities.size(), ComponentMask() });
		return entities.back().id;
	}

	template<typename Component>
	Component* AssignComponent(Entity entity)
	{
		int componentID = GetId<Component>();

		// ensure there enough space
		if (componentPools.size()  <= componentID)
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

	template<typename Component>
	Component* Get(Entity entity)
	{
		int componentID = GetId<Component>();
		if (!entities[entity].mask.test(componentID))
			return nullptr;

		Component* comp = static_cast<Component*>(componentPools[componentID]->get(entity));
		return comp;
	}

public:
	std::vector<EntityDesc> entities;
	std::vector<ComponentPool*> componentPools;
};