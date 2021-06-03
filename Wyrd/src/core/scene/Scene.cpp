#pragma once
#include "wyrdpch.h"

/* Local includes */
#include "Scene.h"
#include "core/ecs/Components.h"
#include "serial/ComponentSerialiserFactory.h"
#include "serial/TypeSerialisers.h"

#include <jsonxx.h>

namespace Wyrd
{
	Scene::Scene() : name("Untitled Scene") 
	{
		bgcolor.r = 0.2f;
		bgcolor.g = 0.2f;
		bgcolor.b = 0.2f;
		bgcolor.a = 1.0f;
	}

	Scene::Scene(std::string name) : name(name) 
	{
		bgcolor.r = 0.2f;
		bgcolor.g = 0.2f;
		bgcolor.b = 0.2f;
		bgcolor.a = 1.0f;
	}

	bool Scene::Initialise()
	{
		/* register all the inbuild components with the ECS */
		RegisterComponent<MetaDataComponent>("MetaData", "MetaDataComponent");
		RegisterComponent<Transform2DComponent>("Transform2D", "Transform2DComponent");
		RegisterComponent<SpriteComponent>("Sprite", "SpriteComponent");
		RegisterComponent<ScriptComponent>("Script", "ScriptComponent");
		RegisterComponent<CameraComponent>("Camera", "CameraComponent");
		return true;
	}

	void Scene::AssignScripts(Behaviour* behaviour)
	{

	}
	
	bool Scene::ToJson(jsonxx::Object& object)
	{
		/* background color */
		object << "bgcolor" << bgcolor;

		/* camera position */
		object << "cameraPosition" << cameraPosition;

		/* camera zoom */
		object << "cameraZoom" << cameraZoom;

		/* primary camerauid */
		object << "primaryCameraUID" << _ScenePrimaryCamera.str();

		/* entity list */
		jsonxx::Array entityArray;

		for (auto& e : entities)
		{
			jsonxx::Object entity;
			entity << "id" << e.id;
			entity << "mask" << e.mask.to_ullong();
			entityArray << entity;
		}

		object << "entities" << entityArray;


		/* component pools */
		jsonxx::Array poolArray;
		for (auto& p : componentPools)
		{
			jsonxx::Object pool;

			pool << "name" << p->name;
			pool << "elementSize" << p->elementSize;

			jsonxx::Array components;

			for (size_t i = 0; i < entities.size(); i++)
			{
				components <<  ComponentSerialiserFactory::Serialise(p->name, &p->data[i * p->elementSize]);
				pool << "components" << components;
			}

			poolArray << pool;
		}

		object << "componentPools" << poolArray;

		return true;
	}

	bool Scene::FromJson(jsonxx::Object& object)
	{
		/* background color */
		if (object.has<jsonxx::Array>("bgcolor"))
			bgcolor << object.get<jsonxx::Array>("bgcolor");

		/* camera position */
		if (object.has<jsonxx::Array>("cameraPosition"))
			cameraPosition << object.get<jsonxx::Array>("cameraPosition");

		/* camera zoom */
		if (object.has<jsonxx::Number>("cameraZoom"))
			cameraZoom = (float)object.get<jsonxx::Number>("cameraZoom");

		/* primary camera uid*/
		if (object.has<jsonxx::String>("primaryCameraUID"))
			_ScenePrimaryCamera = UID(object.get<jsonxx::String>("primaryCameraUID"));

		/* entity list */
		if (object.has<jsonxx::Array>("entities"))
		{
			jsonxx::Array entityArray = object.get<jsonxx::Array>("entities");
		
			for (size_t i = 0; i < entityArray.size(); i++)
			{
				jsonxx::Object entity = entityArray.get<jsonxx::Object>((unsigned int)i);
				Entity id = (Entity)entity.get<jsonxx::Number>("id");
				ComponentMask mask = (ComponentMask)entity.get<jsonxx::Number>("mask");
				
				/* check to make sure there is space to store this entity */
				if (id > entities.size())
					entities.resize(id);
		
				entities[id - 1] = { id, mask };
			}
		}
		
		/* component pools */
		if (object.has<jsonxx::Array>("componentPools"))
		{
			jsonxx::Array componentPoolsArray = object.get<jsonxx::Array>("componentPools");
			for (size_t i = 0; i < componentPoolsArray.size(); i++)
			{
				jsonxx::Object pool = componentPoolsArray.get<jsonxx::Object>((unsigned int)i);
		
				std::string poolName = pool.get<String>("name");
				int poolElementSize = pool.get<Number>("elementSize"); 
		
				jsonxx::Array compnentArray = pool.get<jsonxx::Array>("components");
				for (size_t j = 0; j < compnentArray.size(); j++)
				{
					jsonxx::Object component = compnentArray.get<jsonxx::Object>((unsigned int)j);
		
					ComponentSerialiserFactory::Deserialise(poolName, component, &(componentPools[i]->data[j * poolElementSize]));
					componentPools[i]->count++;
				}
			}
		}

		return true;
	}

	Entity Scene::CreateEntity()
	{
		entities.push_back({ entities.size() + 1, ComponentMask() });
		return entities.back().id;
	}

	void Scene::DestroyEntity(Entity entity)
	{
		// remove the components from the pool
		for (auto& p : componentPools)
		{
			memcpy(p->data + ((entity-1)*p->elementSize), p->data + (entity * p->elementSize), p->elementSize * (MAX_ENTITIES - entity));
		}
		
		// remove entity from list
		entities.erase(entities.begin() + (entity-1));

		// update id in remaining entities
		for (int i = (entity - 1); i < entities.size(); ++i)
		{
			entities[i].id--;
		}
	}

	void Scene::SwapEntity(Entity entityA, Entity entityB)
	{
		// swap components
		for (auto& p : componentPools)
		{
			char* a = p->data + ((entityA - 1) * p->elementSize);
			char* b = p->data + ((entityB - 1) * p->elementSize);
			char* t = p->data + (p->elementSize * MAX_ENTITIES);

			memcpy(t, a, p->elementSize);
			memcpy(a, b, p->elementSize);
			memcpy(b, t, p->elementSize);
		}
	}
}