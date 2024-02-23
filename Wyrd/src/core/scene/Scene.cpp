#include "wyrdpch.h"

/* Local includes */
#include "Scene.h"
#include "core/Log.h"
#include "core/Application.h"
#include "core/behaviour/ScriptedClass.h"
#include "core/ecs/Components.h"
#include "core/ecs/EntitySet.h"
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
		/** TODO: This should really be handled during the code generate for the engine.
		*	However to do that we would need to indentify Core and gameplay components
		*/
		RegisterComponent<MetaDataComponent>("MetaData", "MetaDataComponent", true, &MetaDataComponent::ResetFunc);
		RegisterComponent<Transform2DComponent>("Transform2D", "Transform2DComponent", true, &Transform2DComponent::ResetFunc);
		RegisterComponent<Transform3DComponent>("Transform3D", "Transform3DComponent", true, &Transform3DComponent::ResetFunc);
		RegisterComponent<RelationshipComponent>("Relationship", "RelationshipComponent", true, &RelationshipComponent::ResetFunc);
		RegisterComponent<MeshRendererComponent>("MeshRenderer", "MeshRendererComponent", true, &MeshRendererComponent::ResetFunc);
		RegisterComponent<MaterialComponent>("Material", "MaterialComponent", true, &MaterialComponent::ResetFunc);
		RegisterComponent<SpriteComponent>("Sprite", "SpriteComponent", true, &SpriteComponent::ResetFunc);
		RegisterComponent<ScriptComponent>("Script", "ScriptComponent", true, &ScriptComponent::ResetFunc);
		RegisterComponent<CameraComponent>("Camera", "CameraComponent", true, &CameraComponent::ResetFunc);

		return true;
	}

	void Scene::AssignScripts(const Entity entity)
	{
		//ScriptComponent* scriptComponent = Get<ScriptComponent>(entity);
		//if (scriptComponent != nullptr)
		//{
		//	auto scriptedClass = Application::Get().GetBehaviour().GetCustomClassByUID(scriptComponent->script);
		//
		//	if (scriptedClass != nullptr)
		//	{
		//		scriptComponent->propertyMap.clear();
		//
		//		int i = 0;
		//		for (auto& o : scriptedClass->Properties)
		//		{
		//			scriptComponent->propertyMap.insert({ o.second->GetName(),  (scriptComponent->properties + (i * SCRIPT_COMP_PROP_DATA_LENGTH)) });
		//			i++;
		//		}
		//	}
		//	else
		//	{
		//		WYRD_CORE_ERROR("Unable to find matching class!");
		//	}
		//}
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

		return ent;
	}

	Entity Scene::CreateEntity(const std::string& name)
	{
		Entity ent = CreateEntity();

		/* register all the inbuild components with the ECS */
		InitialiseComponent<MetaDataComponent>(ent);
		InitialiseComponent<RelationshipComponent>(ent);
		InitialiseComponent<Transform3DComponent>(ent);
		
		// Assign all initial components
		MetaDataComponent* metaDataComponent = AssignComponent<MetaDataComponent>(ent);
		RelationshipComponent* relationshipComponent = AssignComponent<RelationshipComponent>(ent);
		Transform3DComponent* transform3DComponent = AssignComponent<Transform3DComponent>(ent);

		strcpy(metaDataComponent->name, name.c_str());

		return ent;
	}

	void Scene::DestroyEntity(Entity entity)
	{
		entities[entity - 1].mask.reset();

		_AvailableEntities.push_back(entity);

		// remove the components from the pool
		//for (auto& p : componentPools)
		//{
		//	memcpy(p->data + ((entity - 1) * p->elementSize), p->data + (entity * p->elementSize), p->elementSize * (MAX_ENTITIES - entity));
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

	void Scene::CleanUp()
	{
		for (Entity e : EntitySet<RelationshipComponent>(*this))
		{
			RelationshipComponent* relationshipComponent = Get<RelationshipComponent>(e);

			if (relationshipComponent->remove == true)
			{
				DestroyEntity(e);
				relationshipComponent->remove = false;
				break;
			}
		}
	}

	void Scene::RemoveComponent(uint32_t poolIndex, Entity entity)
	{
		entities[entity - 1].mask.set(poolIndex, false);
		componentPools[poolIndex]->resetFunc(componentPools[poolIndex], entity);
		//memset(componentPools[poolIndex]->get(entity), 0, componentPools[poolIndex]->elementSize);
		//componentPools[poolIndex]->count--;
	}
}