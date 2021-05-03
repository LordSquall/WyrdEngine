#pragma once

/* core wyrd includes */
#include <wyrdpch.h>
#include <core/Log.h>
#include <core/Application.h>
#include <core/scene/Scene.h>
#include <core/scene/Layer2D.h>
#include <core/scene/GameObject.h>
#include <core/scene/components/Transform2DComponent.h>
#include <core/scene/components/Transform3DComponent.h>
#include <core/scene/components/SpriteComponent.h>
#include <core/scene/components/ScriptComponent.h>
#include <core/scene/components/PhysicsComponent.h>
#include <core/renderer/Texture.h>

/* local includes */
#include "SceneLoader.h"
#include "services/ServiceManager.h"
#include "services/ResourceService.h"

/* external includes */
#include <glm/glm.hpp>
#include <jsonxx.h>

using namespace glm;

namespace Wyrd::Editor
{
	static std::shared_ptr<ResourceService> _resourceService;
	static PropertyList_t _resolveProperties;

	SceneLoader::Result SceneLoader::Load(std::string path, Scene& scene)
	{
		SceneLoader::Result result = Success;
		jsonxx::Object o;

		_resourceService = ServiceManager::Get<ResourceService>(ServiceManager::Service::Resources);

		std::ifstream f(path);

		if (f.is_open() == true) {
			std::ostringstream ss;
			ss << f.rdbuf();

			if (o.parse(ss.str()) == true)
			{
				scene.FromJson(o);

				/* we want to use the file name as the scene name */
				scene.name = Utils::GetFilename(path);

				/* we need to process of linked properties */
				for (auto& layer : scene.GetLayers())
				{
					for (auto& go : layer->GetGameObjects())
					{
						for (auto& comp : go->components)
						{
							ScriptComponent* script = dynamic_cast<ScriptComponent*>(comp.get());

							if (script != nullptr)
							{
								for (auto& prop : *script->Properties)
								{
									prop.second->Resolve(scene);
								}
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

		return result;
	}

	SceneLoader::Result SceneLoader::Save(std::string path, Scene& scene)
	{
		SceneLoader::Result result = Success;

		jsonxx::Object o = scene.ToJson();

		std::ofstream out(path);
		out << o.json();
		out.close();

		return result;
	}
}
