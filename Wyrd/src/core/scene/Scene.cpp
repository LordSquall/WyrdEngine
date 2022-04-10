#pragma once
#include "wyrdpch.h"

/* Local includes */
#include "Scene.h"
#include "core/Log.h"
#include "core/Application.h"
#include "core/behaviour/Behaviour.h"
#include "core/behaviour/ScriptedClass.h"
#include "core/ecs/Components.h"
#include "serial/ComponentSerialiserFactory.h"
#include "serial/TypeSerialisers.h"

namespace Wyrd
{
	Scene::Scene() : name("Untitled Scene") 
	{
		bgcolor.r = 0.2f;
		bgcolor.g = 0.2f;
		bgcolor.b = 0.2f;
		bgcolor.a = 1.0f;

		_ScenePrimaryCameraEntity = ENTITY_INVALID;
	}

	Scene::Scene(std::string name) : name(name) 
	{
		bgcolor.r = 0.2f;
		bgcolor.g = 0.2f;
		bgcolor.b = 0.2f;
		bgcolor.a = 1.0f;

		_ScenePrimaryCameraEntity = ENTITY_INVALID;
	}

	bool Scene::Initialise()
	{
		/* register all the inbuild components with the ECS */
		RegisterComponent<MetaDataComponent>("MetaData", "MetaDataComponent", true);
		RegisterComponent<Transform2DComponent>("Transform2D", "Transform2DComponent", true);
		RegisterComponent<SpriteComponent>("Sprite", "SpriteComponent", true);
		RegisterComponent<ScriptComponent>("Script", "ScriptComponent", true);
		RegisterComponent<CameraComponent>("Camera", "CameraComponent", true);
		RegisterComponent<TextComponent>("Text", "TextComponent", true);
		RegisterComponent<RelationshipComponent>("Relationship", "RelationshipComponent", true);
		return true;
	}

	void Scene::AssignScripts(const Entity entity)
	{
		ScriptComponent* scriptComponent = Get<ScriptComponent>(entity);
		if (scriptComponent != nullptr)
		{
			auto scriptedClass = Application::Get().GetBehaviour().GetCustomClassByUID(scriptComponent->script);

			if (scriptedClass != nullptr)
			{
				scriptComponent->propertyMap.clear();

				int i = 0;
				for (auto& o : scriptedClass->Properties)
				{
					scriptComponent->propertyMap.insert({ o.second->GetName(),  (scriptComponent->properties + (i * SCRIPT_COMP_PROP_DATA_LENGTH)) });
					i++;
				}
			}
			else
			{
				WYRD_CORE_ERROR("Unable to find matching class!");
			}
		}
	}
	
	Entity Scene::CreateEntity()
	{
		Entity ent = ENTITY_INVALID;
		if (_AvailableEntities.empty() == false)
		{
			ent = *_AvailableEntities.begin();
			_AvailableEntities.erase(_AvailableEntities.begin());
		}
		else
		{
			entities.push_back({ entities.size() + 1, ComponentMask() });
			ent = entities.back().id;
		}
		
		/* register all the inbuild components with the ECS */
		InitialiseComponent<MetaDataComponent>(ent);
		InitialiseComponent<Transform2DComponent>(ent);
		InitialiseComponent<SpriteComponent>(ent);
		InitialiseComponent<ScriptComponent>(ent);
		InitialiseComponent<CameraComponent>(ent);
		InitialiseComponent<TextComponent>(ent);
		InitialiseComponent<RelationshipComponent>(ent);

		return ent;
	}

	void Scene::DestroyEntity(Entity entity)
	{
		entities[entity - 1].mask.reset();

		_AvailableEntities.push_back(entity);

		//// remove the components from the pool
		//for (auto& p : componentPools)
		//{
		//	memcpy(p->data + ((entity - 1) * p->elementSize), p->data + (entity * p->elementSize), p->elementSize * (MAX_ENTITIES - entity));
		//}

		//// remove entity from list
		//entities.erase(entities.begin() + (entity - 1));

		//// update id in remaining entities
		//for (int i = (entity - 1); i < entities.size(); ++i)
		//{
		//	entities[i].id--;
		//}
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

		// swap component masks
		ComponentMask tempMask = entities[entityA - 1].mask;
		entities[entityA - 1].mask = entities[entityB - 1].mask;
		entities[entityB - 1].mask = tempMask;

		// Update script components (if applicable)
		AssignScripts(entityA);
		AssignScripts(entityB);
	}
}