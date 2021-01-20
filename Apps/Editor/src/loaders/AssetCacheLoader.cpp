#pragma once

/* core osiris includes */
#include <core/Log.h>

/* local includes */
#include "loaders/AssetCacheLoader.h"
#include "services/ServiceManager.h"
#include "services/ResourceService.h"
#include "datamodels/resources/SceneRes.h"

/* external includes */
#include <jsonxx.h>

namespace Osiris::Editor
{
	std::shared_ptr<ResourceService> _ResourceService = nullptr;

	AssetCacheLoader::Result AssetCacheLoader::Load(std::string path)
	{
		AssetCacheLoader::Result result = Success;
		jsonxx::Object o;

		/* cache the resource service */
		_ResourceService = ServiceManager::Get<ResourceService>(ServiceManager::Resources);

		std::ifstream f(path);

		if (f.is_open() == true) {
			std::ostringstream ss;
			ss << f.rdbuf();

			if (o.parse(ss.str()) == true)
			{
				jsonxx::Array resourcesArray = o.get<jsonxx::Array>("assets");

				for (int i = 0; i < resourcesArray.size(); i++)
				{
					jsonxx::Object cacheEntry = resourcesArray.get<jsonxx::Object>(i);

					jsonxx::String filePath = Utils::GetAssetFolder() + "\\" + cacheEntry.get<jsonxx::String>("filepath");
					jsonxx::String name = cacheEntry.get<jsonxx::String>("name");
					jsonxx::String uuid = cacheEntry.get<jsonxx::String>("uuid");
					jsonxx::String hash = cacheEntry.get<jsonxx::String>("hash");

					/* check the underlying file is present */
					if (Utils::FileExists(filePath))
					{
						/* load and add as a new resource */
						_ResourceService->AddResource(filePath, UID(uuid));
					}
					else
					{
						OSR_TRACE("Missing File from cache: {0}", filePath);
					}

					_ResourceService->CachedFiles[filePath] = UID(uuid);
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

	AssetCacheLoader::Result AssetCacheLoader::Save(std::string path)
	{
		AssetCacheLoader::Result result = Success;
		jsonxx::Object o;

		/* cache the resource service */
		_ResourceService = ServiceManager::Get<ResourceService>(ServiceManager::Resources);

		jsonxx::Array resourceArray;
		for (auto& resource : _ResourceService->GetResources())
		{
			auto res = resource.second;

			jsonxx::Object sceneObj;

			sceneObj << "name" << res->GetName();
			sceneObj << "filepath" << res->GetPath().substr((Utils::GetAssetFolder() + "\\").length());
			sceneObj << "uuid" << res->GetResourceID().str();
			sceneObj << "hash" << Utils::HashFile(res->GetPath());

			resourceArray << sceneObj;
		}

		o << "assets" << resourceArray;

		std::ofstream out(path);
		out << o.json();
		out.close();

		return result;
	}
}
