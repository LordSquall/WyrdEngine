/* core wyrd includes */
#include <wyrdpch.h>
#include <core/Log.h>
#include <core/Application.h>
#include <core/Resources.h>
#include <core/renderer/Shader.h>
#include <core/renderer/Mesh.h>
#include <core/renderer/FontType.h>

/* local includes */
#include "ResourceService.h"
#include "services/ServiceManager.h"
#include "serial/LogEditorTypeFunctions.h"
#include "loaders/AssetCacheLoader.h"
#include "loaders/ResourceSetLoader.h"
#include "loaders/TextureLoader.h"
#include "loaders/ObjLoader.h"
#include "datamodels/resources/TextureRes.h"


namespace Wyrd::Editor
{
	void ResourceService::OnCreate()
	{
		/* Cache the service pointers */
		_WorkspaceService = ServiceManager::Get<WorkspaceService>();

		/* Register the ignored file extensions */
		_ignoredExtensions.insert(".dll");
		_ignoredExtensions.insert(".mdb");
		_ignoredExtensions.insert(".spritedata");

		/* Register the raw file extensions. These will be directly importable to the editor and create a new resource cache file */
		_extensions.insert(std::pair<std::string, ResourceType>(".png", ResourceType::TEXTURE));
		_extensions.insert(std::pair<std::string, ResourceType>(".jpg", ResourceType::TEXTURE));
		_extensions.insert(std::pair<std::string, ResourceType>(".bmp", ResourceType::TEXTURE));
		_extensions.insert(std::pair<std::string, ResourceType>(".shader", ResourceType::SHADER));
		_extensions.insert(std::pair<std::string, ResourceType>(".vs", ResourceType::SHADERSTAGE_VS));
		_extensions.insert(std::pair<std::string, ResourceType>(".fs", ResourceType::SHADERSTAGE_FS));
		_extensions.insert(std::pair<std::string, ResourceType>(".cs", ResourceType::SCRIPT));
		_extensions.insert(std::pair<std::string, ResourceType>(".ttf", ResourceType::FONT));

		/* Register the cache file extensions for each of the imported resource types */
		_extensions.insert(std::pair<std::string, ResourceType>(".texture", ResourceType::TEXTURE));
		_extensions.insert(std::pair<std::string, ResourceType>(".shader", ResourceType::SHADER));
		_extensions.insert(std::pair<std::string, ResourceType>(".scene", ResourceType::SCENE));
		_extensions.insert(std::pair<std::string, ResourceType>(".script", ResourceType::SCRIPT));
		_extensions.insert(std::pair<std::string, ResourceType>(".material", ResourceType::MATERIAL));
		_extensions.insert(std::pair<std::string, ResourceType>(".obj", ResourceType::MODEL));

		/* Load the Editor Resources */
		ResourceSetLoader::Result result = ResourceSetLoader::Load(Utils::GetEditorResFolder() + "editorResources.json", _resourceMap);
		if (result != ResourceSetLoader::Result::Success)
		{
			//WYRD_CORE_ERROR("Failed to load editorResources! cause: {0}", result);
		}

		/* add the default shaders */
		{
			/* default font(s) */
			for (const auto& entry : std::filesystem::directory_iterator(Utils::GetEditorResFolder() + "fonts"))
			{
				if (entry.is_regular_file() && entry.path().extension() == ".ttf")
				{
					AddResource(entry.path().string(), UID());
				}
			}
		}

		/* Set the fallback material */
		//Application::Get().GetRenderer().SetFallbackMaterial(Application::Get().GetResources().Materials[RES_FALLBACK_MATERIAL]);

		/* Load default icons into the icon library */
		_iconLibrary.AddIconsFromFile(Utils::GetEditorResFolder() + "icons/common-icons.json");

		/* Subscribe to project events */
		ServiceManager::Get<EventService>()->Subscribe(Events::EventType::ProjectLoaded, WYRD_BIND_FN(ResourceService::OnProjectLoadedEvent));
		ServiceManager::Get<EventService>()->Subscribe(Events::EventType::AddFileEntry, WYRD_BIND_FN(ResourceService::OnAddFileEntryEvent));
		ServiceManager::Get<EventService>()->Subscribe(Events::EventType::DeleteFileEntry, WYRD_BIND_FN(ResourceService::OnDeleteFileEntryEvent));
		ServiceManager::Get<EventService>()->Subscribe(Events::EventType::RenameFileEntry, WYRD_BIND_FN(ResourceService::OnReloadFileEntryEvent));
		ServiceManager::Get<EventService>()->Subscribe(Events::EventType::ModifiedFileEntry, WYRD_BIND_FN(ResourceService::OnModifiedFileEntryEvent));
		ServiceManager::Get<EventService>()->Subscribe(Events::EventType::LoadAsset, WYRD_BIND_FN(ResourceService::OnLoadAssetEvent));
	}

	void ResourceService::OnDestroy() {}

	void ResourceService::OnUpdate()
	{
		if (!_loadableResources.empty())
		{
			_loadableResources.top()->Load(_loadableResources.top()->GetPath().string());
			_loadableResources.pop();
		}
	}

	void ResourceService::AddResource(const std::filesystem::path& resourcePath, const UID uid)
	{
		if (CheckIgnored(resourcePath) == false)
		{
			/* determine the type of resource from the path */
			ResourceType resourceType = DetermineType(resourcePath);

			/* if we can't resolve a type, then we can skip the resource */
			if (resourceType != ResourceType::NONE)
			{
				/* create a new resource object */
				std::shared_ptr<Resource> resource = ResourceFactory::Create(resourceType, resourcePath, uid);
				resource->SetPath(resourcePath);

				/* add to the resource map */
				_resourceMap.insert(std::pair<UID, std::shared_ptr<Resource>>(resource->GetResourceID(), resource));

				/* load all resources for now */
				Resource::IOResult result = resource->Load(resourcePath.string());
				if (result != Resource::IOResult::Success)
				{
					WYRD_CORE_ERROR("Unable to Load Resource: {0}", (int)result);
				}

				/* add the cachedFiles */
				CachedFiles[resourcePath] = uid;
			}
		}
		else
		{
			//WYRD_CORE_INFO("Skipping Asset file: [{0}] - As extension is on the ignored list!", resourcePath);
		}
	}

	void ResourceService::ReloadResource(UID uid)
	{

	}

	void ResourceService::DeleteResource(UID uid)
	{
		// unload if required TODO

		_resourceMap.erase(uid);
	}

	std::map<UID, std::shared_ptr<Resource>> ResourceService::GetResourcesByDir(const std::string& dir)
	{
		std::map<UID, std::shared_ptr<Resource>> result;

		std::filesystem::path compDir(dir);
		for (auto&& res : _resourceMap)
		{
			std::filesystem::path p(res.second->GetPath());

			if (p.parent_path().string() == compDir.string())
			{
				result.insert(std::pair<UID, std::shared_ptr<Resource>>(res.first, res.second));
			}
		}
		return result;
	}

	std::shared_ptr<Resource> ResourceService::GetSceneResourceByName(const std::string& name)
	{
		std::map<UID, std::shared_ptr<Resource>> result;

		for (auto&& res : _resourceMap)
		{
			std::filesystem::path p(res.second->GetName());

			if (res.second->GetName() == name)
			{
				return res.second;
			}
		}
		return nullptr;
	}

	void ResourceService::BuildScripts()
	{
		/* ensure we compile on the main thread */
		if (Application::Get().GetMainThreadID() != std::this_thread::get_id())
		{
			/* swap an event to trigger the load on the main thread */
			ServiceManager::Get<EventService>()->Publish(Events::EventType::BuildBehaviourModel, std::make_unique<Events::BuildBehaviourModelArgs>());
		}
		else
		{
			bool successfullyCompiled = ServiceManager::Get<SimulationService>()->CompileAll();

			if (successfullyCompiled)
			{
				// Resolve all the script resources to the scripted classes
				for (auto& [key, value] : _resourceMap)
				{
					auto downcastedPtr = std::dynamic_pointer_cast<ScriptRes>(value);
					if (downcastedPtr)
					{
						auto scriptedClass = ServiceManager::Get<SimulationService>()->GetClass(value->GetName());

						/* set the uid of the scripted class to match the resource */
						scriptedClass->SetUID(downcastedPtr->GetResourceID());
						downcastedPtr->Script = scriptedClass;
						downcastedPtr->PendingReload = true;
					}
				}

				/* trigger an event to alert the script components to refresh there property maps  */
				ServiceManager::Get<EventService>()->Publish(Events::EventType::BehaviourModelBuilt, std::make_unique<Events::BehaviourModelBuiltArgs>());
			}
		}
	}

	bool ResourceService::CheckIgnored(const std::filesystem::path& path)
	{
		return (_ignoredExtensions.find(path.extension().string()) != _ignoredExtensions.end());
	}

	ResourceType ResourceService::DetermineType(const std::filesystem::path& path)
	{
		std::map<std::string, ResourceType>::iterator it = _extensions.find(path.extension().string());

		if (it != _extensions.end())
		{
			return it->second;
		}
		else
		{
			return ResourceType::NONE;
		}
	}


	bool ResourceService::LoadAssetCache(const std::string& filePath)
	{
		if (!std::filesystem::exists(filePath))
		{
			WYRD_INFO("Unable to find Asset cache file.");
			return true;
		}

		AssetCacheLoader::Load((_WorkspaceService->GetCacheDirectory() / "assets.cache").string());

		return true;
	}

	void ResourceService::OnProjectLoadedEvent(Events::EventArgs& args)
	{
		Events::ProjectLoadedArgs& a = (Events::ProjectLoadedArgs&)args;

		/* load the asset cache */
		LoadAssetCache((_WorkspaceService->GetCacheDirectory() / "assets.cache").string());

		auto files = Utils::GetFileList(_WorkspaceService->GetAssetsDirectory().string(), true, true);

		for (auto t : files)
		{
			/* check if the file is already present from the cache load */
			if (CachedFiles.find(t) == CachedFiles.end())
			{
				AddResource(t, UIDUtils::Create());
			}
		}

		/* Save the cache file */
		AssetCacheLoader::Save((_WorkspaceService->GetCacheDirectory() / "assets.cache").string());

		/* Once all the resources are loaded, now we can compile the all the scripts */
		BuildScripts();
	}

	void ResourceService::OnAddFileEntryEvent(Events::EventArgs& args)
	{
		Events::AddFileEntryArgs& a = (Events::AddFileEntryArgs&)args;
	
		AddResource(a.filepath, UIDUtils::Create());

		/* Save the cache file */
		AssetCacheLoader::Save((_WorkspaceService->GetCacheDirectory() / "assets.cache").string());
	}

	void ResourceService::OnDeleteFileEntryEvent(Events::EventArgs& args)
	{
		Events::DeleteFileEntryArgs& a = (Events::DeleteFileEntryArgs&)args;

		if (CachedFiles.find(a.filepath) != CachedFiles.end())
		{
			DeleteResource(CachedFiles[a.filepath]);
		}

		/* Save the cache file */
		AssetCacheLoader::Save((_WorkspaceService->GetCacheDirectory() / "assets.cache").string());
	}

	void ResourceService::OnReloadFileEntryEvent(Events::EventArgs& args)
	{
		Events::RenameFileEntryArgs& a = (Events::RenameFileEntryArgs&)args;

		/* if we have a directory change we should update the cached files for all the effected assets */
		if (a.isDir)
		{
			for (auto&& res : _resourceMap)
			{
				std::string oldPath = a.dir + a.previousName;

				//if (res.second->GetPath().rfind(oldPath, 0) == 0)
				//{
				//	/* remove from the cached file list */
				//	CachedFiles.erase(res.second->GetPath());
				//
				//	/* update the path */
				//	std::string newPath = a.filepath + res.second->GetPath().substr(oldPath.length());
				//	res.second->SetPath(newPath);
				//
				//	/* remove from the cached file list */
				//	CachedFiles[newPath] = res.second->GetResourceID();
				//}
			}
		}
		
		/* Save the cache file */
		AssetCacheLoader::Save((_WorkspaceService->GetCacheDirectory() / "assets.cache").string());
	}


	void ResourceService::OnModifiedFileEntryEvent(Events::EventArgs& args)
	{
		Events::ModifiedFileEntryArgs& a = (Events::ModifiedFileEntryArgs&)args;

		/* if we have a change to any script, we need to re-compile the behaviour model */			
		ResourceType resourceType = DetermineType(a.filepath);
		if (resourceType == ResourceType::SCRIPT)
		{
			BuildScripts();
		}
		else 
		{
			std::shared_ptr<Resource> res = GetResourceByFilePath(a.filepath);
			res->Reload();
		}
	}

	void ResourceService::OnLoadAssetEvent(Events::EventArgs& args)
	{
		Events::LoadAssetArgs& a = (Events::LoadAssetArgs&)args;
		
		_loadableResources.push(a.resource);
	}
}
