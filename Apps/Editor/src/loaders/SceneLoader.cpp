/* core wyrd includes */
#include <wyrdpch.h>
#include <core/Log.h>
#include <core/Application.h>
#include <core/scene/Scene.h>
#include <core/renderer/Texture.h>
#include <core/ecs/Components.h>

/* local includes */
#include "SceneLoader.h"
#include "services/ServiceManager.h"
#include "services/ResourceService.h"
#include "serial/ComponentSerialiserFactory.h"
#include "datamodels/EditorComponents.h"
#include "events/EditorEvents.h"
#include "support/TypeSerialisers.h"

/* external includes */
#include <glm/glm.hpp>
#include <jsonxx.h>

using namespace glm;

namespace Wyrd::Editor
{
	static std::shared_ptr<ResourceService> _resourceService;
	static std::shared_ptr<CoreSystemsService> _coreSystemService;
	static PropertyList_t _resolveProperties;

	SceneLoader::Result SceneLoader::Load(const std::filesystem::path& path, Scene& scene, bool includeEditorComponents)
	{
		SceneLoader::Result result = Success;
		jsonxx::Object o;

		_resourceService = ServiceManager::Get<ResourceService>();
		_coreSystemService = ServiceManager::Get<CoreSystemsService>();

		/* perform the initial load of the JSON file */
		std::ifstream f(path);
		if (f.is_open() == true) {
			std::ostringstream ss;
			ss << f.rdbuf();

			if (o.parse(ss.str()) == true)
			{
				/* background color */
				if (o.has<jsonxx::Array>("bgcolor"))
					scene.bgcolor << o.get<jsonxx::Array>("bgcolor");

				/* camera position */
				if (o.has<jsonxx::Array>("cameraPosition"))
					scene.cameraPosition << o.get<jsonxx::Array>("cameraPosition");

				/* camera zoom */
				if (o.has<jsonxx::Number>("cameraZoom"))
					scene.cameraZoom = (float)o.get<jsonxx::Number>("cameraZoom");

				/* scene name */
				if (o.has<jsonxx::String>("name"))
					scene.name = o.get<jsonxx::String>("name");

				/* primary camera uid*/
				if (o.has<jsonxx::Number>("primaryCameraEntity"))
					scene.SetPrimaryCameraEntity((Entity)o.get<jsonxx::Number>("primaryCameraEntity"));

				/* available entities */
				//if (o.has<jsonxx::Array>("availableEntities"))
				//{
				//	jsonxx::Array availableEntities = o.get<jsonxx::Array>("availableEntities");
				//	for (size_t i = 0; i < availableEntities.size(); i++)
				//	{
				//		scene._AvailableEntities.push_back(availableEntities.get<jsonxx::Number>(i));
				//	}
				//}

				/* entity list */
				if (o.has<jsonxx::Array>("entities"))
				{
					jsonxx::Array entityArray = o.get<jsonxx::Array>("entities");

					for (size_t i = 0; i < entityArray.size(); i++)
					{
						jsonxx::Object entity = entityArray.get<jsonxx::Object>((unsigned int)i);
						Entity id = (Entity)entity.get<jsonxx::Number>("id");
						ComponentMask mask = (ComponentMask)entity.get<jsonxx::Number>("mask");

						/* check to make sure there is space to store this entity */
						if (id > scene.entities.size())
							scene.entities.resize(id);

						scene.entities[id - 1] = { id, mask };
					}
				}

				/* component pools */
				if (o.has<jsonxx::Array>("componentPools"))
				{
					jsonxx::Array componentPoolsArray = o.get<jsonxx::Array>("componentPools");
					for (size_t i = 0; i < componentPoolsArray.size(); i++)
					{
						jsonxx::Object poolObj = componentPoolsArray.get<jsonxx::Object>((unsigned int)i);
				
						std::string poolName = poolObj.get<jsonxx::String>("name");
				
						/* find the component pool */
						auto foundPoolIt = std::find_if(scene.componentPools.begin(), scene.componentPools.end(),
							[&poolName](const ComponentPool* p) { return p->name.compare(poolName) == 0; }
						);
				
						if (foundPoolIt == scene.componentPools.end())
						{
							continue;
						}
				
						ComponentPool* pool = *foundPoolIt;
				
						int poolElementSize = pool->elementSize;
				
						if (poolObj.has<jsonxx::Array>("components"))
						{
							jsonxx::Array compnentArray = poolObj.get<jsonxx::Array>("components");
							for (size_t j = 0; j < compnentArray.size(); j++)
							{
								jsonxx::Object component = compnentArray.get<jsonxx::Object>((unsigned int)j);
				
								ComponentSerialiserFactory::Deserialise(poolName, component, &(pool->data[j * poolElementSize]));
								pool->count++;
							}
						}
					}
				}
			}
			else
			{
				result = FileMalformed;
			}
		}
		else
		{
			result = FileNotFound;
		}

		/* add additional editor components */
		if (includeEditorComponents)
		{
			for (auto& entity : scene.entities)
			{
				if (entity.mask != 0)
					scene.AssignComponent<EditorComponent>(entity.id);
			}
		}

		/* set the main camera */
		if (scene.GetPrimaryCameraEntity() != ENTITY_INVALID)
		{
			//CameraComponent* cameraComponent = scene.Get<CameraComponent>(scene.GetPrimaryCameraEntity());
			//if (cameraComponent != nullptr)
			//{
			//	ServiceManager::Get<EventService>()->Publish(Editor::Events::EventType::SetSceneCamera, std::make_unique<Events::SetSceneCameraArgs>(scene.GetPrimaryCameraEntity(), cameraComponent));
			//}
			//else
			//{
			//	WYRD_ERROR("Selected Camera Entity missing Camera component.");
			//}
		}
		else
		{
			WYRD_ERROR("Unable to find Selected Camera Entity.");
		}

		return result;
	}

	SceneLoader::Result SceneLoader::Save(const std::filesystem::path& path, Scene& scene)
	{
		SceneLoader::Result result = Success;

		/* construct the scene into a json file */
		jsonxx::Object o;

		/* background color */
		o << "bgcolor" << scene.bgcolor;

		/* camera position */
		o << "cameraPosition" << scene.cameraPosition;

		/* camera zoom */
		o << "cameraZoom" << scene.cameraZoom;

		/* scene name */
		o << "name" << scene.name;

		/* primary camera entity */
		o << "primaryCameraEntity" << scene.GetPrimaryCameraEntity();

		/* available entities */
		//jsonxx::Array availableEntities;
		//for (auto& ae : scene._AvailableEntities)
		//{
		//	availableEntities << ae;
		//}
		//o << "availableEntities" << availableEntities;


		/* entity list */
		jsonxx::Array entityArray;

		for (auto& e : scene.entities)
		{
			jsonxx::Object entity;
			entity << "id" << e.id;
			entity << "mask" << e.mask.to_ullong();
			entityArray << entity;
		}

		o << "entities" << entityArray;

		/* component pools */
		jsonxx::Array poolArray;
		for (auto& p : scene.componentPools)
		{
			if (p->serialise)
			{
				jsonxx::Object pool;
		
				pool << "name" << p->name;
				pool << "elementSize" << p->elementSize;
		
				jsonxx::Array components;
		
				for (size_t i = 0; i < scene.entities.size(); i++)
				{
					components << ComponentSerialiserFactory::Serialise(p->name, &p->data[i * p->elementSize]);
					pool << "components" << components;
				}
		
				poolArray << pool;
			}
		}
		
		o << "componentPools" << poolArray;

		std::ofstream out(path);
		out << o.json();
		out.close();

		return result;
	}
}
