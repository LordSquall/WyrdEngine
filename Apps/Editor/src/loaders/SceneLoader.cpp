#pragma once

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

/* external includes */
#include <glm/glm.hpp>
#include <jsonxx.h>

using namespace glm;

namespace Wyrd::Editor
{
	static std::shared_ptr<ResourceService> _resourceService;
	static std::shared_ptr<CoreSystemsService> _coreSystemService;
	static PropertyList_t _resolveProperties;

	SceneLoader::Result SceneLoader::Load(std::string path, Scene& scene)
	{
		SceneLoader::Result result = Success;
		jsonxx::Object o;

		_resourceService = ServiceManager::Get<ResourceService>(ServiceManager::Service::Resources);
		_coreSystemService = ServiceManager::Get<CoreSystemsService>(ServiceManager::Service::CoreSystems);

		std::ifstream f(path);

		if (f.is_open() == true) {
			std::ostringstream ss;
			ss << f.rdbuf();

			if (o.parse(ss.str()) == true)
			{
				scene.FromJson(o);
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

		/* construct the scene into a json file */
		jsonxx::Object o;
		scene.ToJson(o);

		std::ofstream out(path);
		out << o.json();
		out.close();

		return result;
	}
}
