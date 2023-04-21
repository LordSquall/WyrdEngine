#include "wyrdpch.h"

/* Local includes */
#include "Scene.h"
#include "core/Log.h"
#include "core/Application.h"
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
		/** TODO: This should really be handled during the code generate for the engine.
		*	However to do that we would need to indentify Core and gameplay components
		*/
		RegisterComponent<MetaDataComponent>("MetaData", "MetaDataComponent", true);
		RegisterComponent<Transform2DComponent>("Transform2D", "Transform2DComponent", true);
		RegisterComponent<Transform3DComponent>("Transform3D", "Transform3DComponent", true);
		RegisterComponent<RelationshipComponent>("Relationship", "RelationshipComponent", true);
		RegisterComponent<MeshComponent>("Mesh", "MeshComponent", true);
		RegisterComponent<SpriteComponent>("Sprite", "SpriteComponent", true);
		RegisterComponent<ScriptComponent>("Script", "ScriptComponent", true);
		RegisterComponent<CameraComponent>("Camera", "CameraComponent", true);

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
	
	// bool Scene::ToJson(jsonxx::Object& object)
	// {
		// /* background color */
		// object << "bgcolor" << bgcolor;
// 
		// /* camera position */
		// object << "cameraPosition" << cameraPosition;
// 
		// /* camera zoom */
		// object << "cameraZoom" << cameraZoom;
// 
		// /* scene name */
		// object << "name" << name;
// 
		// /* primary camera entity */
		// object << "primaryCameraEntity" << _ScenePrimaryCameraEntity;
// 
		// /* available entities */
		// jsonxx::Array availableEntities;
		// for (auto& ae : _AvailableEntities)
		// {
			// availableEntities << ae;
		// }
		// object << "availableEntities" << availableEntities;
// 
// 
		// /* entity list */
		// jsonxx::Array entityArray;
// 
		// for (auto& e : entities)
		// {
			// jsonxx::Object entity;
			// entity << "id" << e.id;
			// entity << "mask" << e.mask.to_ullong();
			// entityArray << entity;
		// }
// 
		// object << "entities" << entityArray;
// 
// 
		// /* component pools */
		// jsonxx::Array poolArray;
		// for (auto& p : componentPools)
		// {
			// if (p->serialise)
			// {
				// jsonxx::Object pool;
// 
				// pool << "name" << p->name;
				// pool << "elementSize" << p->elementSize;
// 
				// jsonxx::Array components;
// 
				// for (size_t i = 0; i < entities.size(); i++)
				// {
					// components << ComponentSerialiserFactory::Serialise(p->name, &p->data[i * p->elementSize]);
					// pool << "components" << components;
				// }
// 
				// poolArray << pool;
			// }
		// }
// 
		// object << "componentPools" << poolArray;
// 
		// return true;
	// }
// 
	// bool Scene::FromJson(jsonxx::Object& object)
	// {
		// /* background color */
		// if (object.has<jsonxx::Array>("bgcolor"))
			// bgcolor << object.get<jsonxx::Array>("bgcolor");
// 
		// /* camera position */
		// if (object.has<jsonxx::Array>("cameraPosition"))
			// cameraPosition << object.get<jsonxx::Array>("cameraPosition");
// 
		// /* camera zoom */
		// if (object.has<jsonxx::Number>("cameraZoom"))
			// cameraZoom = (float)object.get<jsonxx::Number>("cameraZoom");
// 
		// /* scene name */
		// if (object.has<jsonxx::String>("name"))
			// name = object.get<jsonxx::String>("name");
// 
		// /* primary camera uid*/
		// if (object.has<jsonxx::Number>("primaryCameraEntity"))
			// _ScenePrimaryCameraEntity = (Entity)object.get<jsonxx::Number>("primaryCameraEntity");
// 
		// /* available entities */
		// if (object.has<jsonxx::Array>("availableEntities"))
		// {
			// jsonxx::Array availableEntities = object.get<jsonxx::Array>("availableEntities");
			// for (size_t i = 0; i < availableEntities.size(); i++)
			// {
				// _AvailableEntities.push_back(availableEntities.get<jsonxx::Number>(i));
			// }
		// }
// 
		// /* entity list */
		// if (object.has<jsonxx::Array>("entities"))
		// {
			// jsonxx::Array entityArray = object.get<jsonxx::Array>("entities");
		// 
			// for (size_t i = 0; i < entityArray.size(); i++)
			// {
				// jsonxx::Object entity = entityArray.get<jsonxx::Object>((unsigned int)i);
				// Entity id = (Entity)entity.get<jsonxx::Number>("id");
				// ComponentMask mask = (ComponentMask)entity.get<jsonxx::Number>("mask");
				// 
				// /* check to make sure there is space to store this entity */
				// if (id > entities.size())
					// entities.resize(id);
		// 
				// entities[id - 1] = { id, mask };
			// }
		// }
		// 
		// /* component pools */
		// if (object.has<jsonxx::Array>("componentPools"))
		// {
			// jsonxx::Array componentPoolsArray = object.get<jsonxx::Array>("componentPools");
			// for (size_t i = 0; i < componentPoolsArray.size(); i++)
			// {
				// jsonxx::Object poolObj = componentPoolsArray.get<jsonxx::Object>((unsigned int)i);
		// 
				// std::string poolName = poolObj.get<String>("name");
// 
				// /* find the component pool */
				// auto foundPoolIt = std::find_if(componentPools.begin(), componentPools.end(),
					// [&poolName](const ComponentPool* p) { return p->name.compare(poolName) == 0; }
				// );
// 
				// if (foundPoolIt == componentPools.end())
				// {
					// return false;
				// }
// 
				// ComponentPool* pool = *foundPoolIt;
// 
				// int poolElementSize = pool->elementSize; 
// 
				// if (poolObj.has<jsonxx::Array>("components"))
				// {
					// jsonxx::Array compnentArray = poolObj.get<jsonxx::Array>("components");
					// for (size_t j = 0; j < compnentArray.size(); j++)
					// {
						// jsonxx::Object component = compnentArray.get<jsonxx::Object>((unsigned int)j);
// 
						// ComponentSerialiserFactory::Deserialise(poolName, component, &(pool->data[j * poolElementSize]));
						// pool->count++;
					// }
				// }
			// }
		// }
// 
		// return true;
	// }

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
		InitialiseComponent<RelationshipComponent>(ent);
		InitialiseComponent<Transform2DComponent>(ent);
		//InitialiseComponent<SpriteComponent>(ent);
		//InitialiseComponent<ScriptComponent>(ent);
		//InitialiseComponent<CameraComponent>(ent);
		//InitialiseComponent<TextComponent>(ent);

		// Assign all initial components
		MetaDataComponent* metaDataComponent = AssignComponent<MetaDataComponent>(ent);
		RelationshipComponent* relationshipComponent = AssignComponent<RelationshipComponent>(ent);
		Transform2DComponent* transform2DComponent = AssignComponent<Transform2DComponent>(ent);

		strcpy(metaDataComponent->name, "New Entity");

		return ent;
	}

	Entity Scene::CreateEntity(const std::string& name)
	{
		Entity ent = CreateEntity();

		/* register all the inbuild components with the ECS */
		InitialiseComponent<MetaDataComponent>(ent);
		InitialiseComponent<RelationshipComponent>(ent);
		InitialiseComponent<Transform2DComponent>(ent);
		
		// Assign all initial components
		MetaDataComponent* metaDataComponent = AssignComponent<MetaDataComponent>(ent);
		RelationshipComponent* relationshipComponent = AssignComponent<RelationshipComponent>(ent);
		Transform2DComponent* transform2DComponent = AssignComponent<Transform2DComponent>(ent);

		strcpy(metaDataComponent->name, name.c_str());

		return ent;
	}

	void Scene::DestroyEntity(Entity entity)
	{
		entities[entity - 1].mask.reset();

		_AvailableEntities.push_back(entity);

		// remove the components from the pool
		for (auto& p : componentPools)
		{
			memcpy(p->data + ((entity - 1) * p->elementSize), p->data + (entity * p->elementSize), p->elementSize * (MAX_ENTITIES - entity));
		}

		// remove entity from list
		entities.erase(entities.begin() + (entity - 1));
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