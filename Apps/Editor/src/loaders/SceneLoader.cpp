/* core wyrd includes */
#include <wyrdpch.h>
#include <core/Log.h>
#include <core/Application.h>
#include <core/scene/Scene.h>
#include <core/renderer/Texture.h>
#include <core/ecs/Components.h>
#include <core/ecs/EntitySet.h>
#include <serial/TypeSerialisers.h>

/* local includes */
#include "SceneLoader.h"
#include "services/ServiceManager.h"
#include "services/ResourceService.h"
#include "serial/ComponentSerialiserFactory.h"
#include "datamodels/EditorComponents.h"
#include "events/EditorEvents.h"

/* external includes */
#include <glm/glm.hpp>
#include <jsonxx.h>

using namespace glm;

namespace Wyrd::Editor
{
	static std::shared_ptr<ResourceService> _resourceService;
	static std::shared_ptr<CoreSystemsService> _coreSystemService;

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
				if (o.has<jsonxx::Object>("bgcolor"))
					scene.bgcolor << o.get<jsonxx::Object>("bgcolor");

				/* camera position */
				if (o.has<jsonxx::Object>("cameraPosition"))
					scene.cameraPosition << o.get<jsonxx::Object>("cameraPosition");

				/* camera zoom */
				if (o.has<jsonxx::Number>("cameraZoom"))
					scene.cameraZoom = (float)o.get<jsonxx::Number>("cameraZoom");

				/* scene name */
				if (o.has<jsonxx::String>("name"))
					scene.name = o.get<jsonxx::String>("name");

				/* primary camera uid*/
				if (o.has<jsonxx::Number>("primaryCameraEntity"))
					scene.SetPrimaryCameraEntity((Entity)o.get<jsonxx::Number>("primaryCameraEntity"));

				/* entity list */
				if (o.has<jsonxx::Array>("entities"))
				{
					jsonxx::Array entitiesArray = o.get<jsonxx::Array>("entities");

					// resize the scene entities array to fit all descriptions
					scene.entities.resize(entitiesArray.size());

					for (size_t i = 0; i < entitiesArray.size(); i++)
					{
						// reset the entity info in the pools
						for (auto& pool : scene.componentPools)
						{
							pool->resetFunc(pool, i+1);
							pool->count++;
						}

						jsonxx::Object entityObj = entitiesArray.get<jsonxx::Object>(i);

						Wyrd::UID entityUID = UID(entityObj.get<jsonxx::String>("uid"));
						jsonxx::Array componentsArray = entityObj.get<jsonxx::Array>("components");

						ComponentMask mask;

						for (size_t ci = 0; ci < componentsArray.size(); ci++)
						{
							jsonxx::Object componentObject = componentsArray.get<jsonxx::Object>(ci);

							std::string name = componentObject.get<jsonxx::String>("name");
							jsonxx::Object data = componentObject.get<jsonxx::Object>("data");

							/* find the component pool */
							auto foundPoolIt = std::find_if(scene.componentPools.begin(), scene.componentPools.end(),
								[&name](const ComponentPool* p) { return p->name.compare(name) == 0; }
							);

							if (foundPoolIt == scene.componentPools.end())
							{
								continue;
							}

							ComponentPool* pool = *foundPoolIt;

							if (pool->serialise == true)
							{
								int poolElementSize = pool->elementSize;

								ComponentSerialiserFactory::Deserialise(name, data, &(pool->data[i * poolElementSize]));

								mask.set(pool->idx);
							}
						}

						scene.entities[i].id = i + 1;
						scene.entities[i].mask = mask;
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

		/* entity list */
		jsonxx::Array entityArray;

		for (auto& e : scene.entities)
		{
			jsonxx::Object entity;

			MetaDataComponent* metaDataComponent = scene.Get<MetaDataComponent>(e.id);
			entity << "uid" << metaDataComponent->uid;

			jsonxx::Array components;

			for (auto& cp : scene.componentPools)
			{
				if (e.mask.test(cp->idx))
				{
					const char* componentData = (const char*)scene.Get(cp->idx, e.id);

					jsonxx::Object componentObj = ComponentSerialiserFactory::Serialise(cp->name, componentData);

					jsonxx::Object componentsDescObj;
					componentsDescObj << "name" << cp->name;
					componentsDescObj << "data" << componentObj;

					components << componentsDescObj;
				}
			}

			entity << "components" << components;

			entityArray << entity;
		}

		o << "entities" << entityArray;

		std::ofstream out(path);
		out << o.json();
		out.close();

		return result;
	}
}
