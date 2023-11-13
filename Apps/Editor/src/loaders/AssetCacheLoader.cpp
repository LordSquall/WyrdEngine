/* core wyrd includes */
#include <core/Log.h>

/* local includes */
#include "loaders/AssetCacheLoader.h"
#include "services/ServiceManager.h"
#include "services/ResourceService.h"
#include "datamodels/resources/SceneRes.h"

/* external includes */
#include <jsonxx.h>

namespace Wyrd::Editor
{
	std::shared_ptr<ResourceService> _ResourceService = nullptr;
	std::shared_ptr<WorkspaceService> _WorkspaceService = nullptr;

	AssetCacheLoader::Result AssetCacheLoader::Load(std::string path)
	{
		AssetCacheLoader::Result result = Success;
		jsonxx::Object o;

		/* cache the required services */
		_ResourceService = ServiceManager::Get<ResourceService>();
		_WorkspaceService = ServiceManager::Get<WorkspaceService>();

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

					jsonxx::String filePath = cacheEntry.get<jsonxx::String>("filepath");
					jsonxx::String name = cacheEntry.get<jsonxx::String>("name");
					jsonxx::String uuid = cacheEntry.get<jsonxx::String>("uuid");

					/* build the complete path include the assets directory */
					std::string fullFilePath = (_WorkspaceService->GetAssetsDirectory() / filePath).string();

					/* check the underlying file is present */
					if (Utils::FileExists(fullFilePath))
					{
						/* load and add as a new resource */
						_ResourceService->AddResource(fullFilePath, UID(uuid));
					}
					else
					{
						WYRD_TRACE("Missing File from cache: {0}", fullFilePath);
					}

					_ResourceService->CachedFiles[fullFilePath] = UID(uuid);
				}

				/* perform any post load operations on assets (e.g. material -> shader bindings etc...) */
				for (auto const& res : _ResourceService->GetResources())
				{
					res.second->ResolveReferences();
					res.second->Build();
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
		_ResourceService = ServiceManager::Get<ResourceService>();
		_WorkspaceService = ServiceManager::Get<WorkspaceService>();

		jsonxx::Array resourceArray;
		for (auto& resource : _ResourceService->GetResources())
		{
			auto res = resource.second;

			if (!res->IsEditorOnly())
			{
				jsonxx::Object sceneObj;

				sceneObj << "name" << res->GetName();
				sceneObj << "filepath" << res->GetPath().string();
				sceneObj << "uuid" << res->GetResourceID().str();

				resourceArray << sceneObj;
			}
		}

		o << "assets" << resourceArray;

		std::ofstream out(path);
		out << o.json();
		out.close();

		return result;
	}
}
