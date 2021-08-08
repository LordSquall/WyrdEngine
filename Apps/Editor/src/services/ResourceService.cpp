#pragma once

/* core wyrd includes */
#include <wyrdpch.h>
#include <core/Log.h>
#include <core/Application.h>
#include <core/Resources.h>
#include <core/renderer/Shader.h>
#include <core/renderer/FontType.h>

/* local includes */
#include "ResourceService.h"
#include "services/ServiceManager.h"
#include "loaders/AssetCacheLoader.h"
#include "loaders/TextureLoader.h"
#include "datamodels/resources/TextureRes.h"

namespace Wyrd::Editor
{
	void ResourceService::OnCreate()
	{
		/* Cache the service pointers */
		_WorkspaceService = ServiceManager::Get<WorkspaceService>(ServiceManager::Workspace);

		/* Register the ignored file extensions */
		_ignoredExtensions.insert(".dll");
		_ignoredExtensions.insert(".mdb");

		/* Register the raw file extensions. These will be directly importable to the editor and create a new resource cache file */
		_extensions.insert(std::pair<std::string, ResourceType>(".png", ResourceType::TEXTURE));
		_extensions.insert(std::pair<std::string, ResourceType>(".jpg", ResourceType::TEXTURE));
		_extensions.insert(std::pair<std::string, ResourceType>(".bmp", ResourceType::TEXTURE));
		_extensions.insert(std::pair<std::string, ResourceType>(".vs", ResourceType::SHADER));
		_extensions.insert(std::pair<std::string, ResourceType>(".fs", ResourceType::SHADER));
		_extensions.insert(std::pair<std::string, ResourceType>(".cs", ResourceType::SCRIPT));
		_extensions.insert(std::pair<std::string, ResourceType>(".ttf", ResourceType::FONT));

		/* Register the cache file extensions for each of the imported resource types */
		_extensions.insert(std::pair<std::string, ResourceType>(".texture", ResourceType::TEXTURE));
		_extensions.insert(std::pair<std::string, ResourceType>(".shader", ResourceType::SHADER));
		_extensions.insert(std::pair<std::string, ResourceType>(".scene", ResourceType::SCENE));
		_extensions.insert(std::pair<std::string, ResourceType>(".script", ResourceType::SCRIPT));


		/* add the default shaders */
		{
			/* default sprite shader */
			std::ifstream vertexStream(Utils::GetEditorResFolder() + "shaders/sprite.vs");
			std::string vertexSrc((std::istreambuf_iterator<char>(vertexStream)), std::istreambuf_iterator<char>());

			std::ifstream fragmentStream(Utils::GetEditorResFolder() + "shaders/sprite.fs");
			std::string fragmentSrc((std::istreambuf_iterator<char>(fragmentStream)), std::istreambuf_iterator<char>());

			std::shared_ptr<Shader> shader = std::shared_ptr<Shader>(Shader::Create());
			shader->Build(vertexSrc, fragmentSrc);
			shader->Bind();
			Application::Get().GetResources().Shaders.insert(std::pair<std::string, std::shared_ptr<Shader>>("Sprite", shader));
		}
		{
			/* default vertex 2D shader */
			std::ifstream vertexStream(Utils::GetEditorResFolder() + "shaders/vertex2D.vs");
			std::string vertexSrc((std::istreambuf_iterator<char>(vertexStream)), std::istreambuf_iterator<char>());

			std::ifstream fragmentStream(Utils::GetEditorResFolder() + "shaders/vertex2D.fs");
			std::string fragmentSrc((std::istreambuf_iterator<char>(fragmentStream)), std::istreambuf_iterator<char>());

			std::shared_ptr<Shader> shader = std::shared_ptr<Shader>(Shader::Create());
			shader->Build(vertexSrc, fragmentSrc);
			shader->Bind();
			Application::Get().GetResources().Shaders.insert(std::pair<std::string, std::shared_ptr<Shader>>("Vertex2D", shader));
		}
		{
			/* default text shader */
			std::ifstream vertexStream(Utils::GetEditorResFolder() + "shaders/text.vs");
			std::string vertexSrc((std::istreambuf_iterator<char>(vertexStream)), std::istreambuf_iterator<char>());

			std::ifstream fragmentStream(Utils::GetEditorResFolder() + "shaders/text.fs");
			std::string fragmentSrc((std::istreambuf_iterator<char>(fragmentStream)), std::istreambuf_iterator<char>());

			std::shared_ptr<Shader> shader = std::shared_ptr<Shader>(Shader::Create());
			shader->Build(vertexSrc, fragmentSrc);
			shader->Bind();
			Application::Get().GetResources().Shaders.insert(std::pair<std::string, std::shared_ptr<Shader>>("Text", shader));
		}
		{
			/* default font(s) */
			for (const auto& entry : std::filesystem::directory_iterator(Utils::GetEditorResFolder() + "fonts"))
			{
				if (entry.is_regular_file() && entry.path().extension() == ".ttf")
				{
					AddResource(entry.path().string(), UID());
					//Application::Get().GetResources().FontTypes.insert(std::pair<std::string, std::shared_ptr<FontType>>(entry.path().filename().stem().string(), FontType::CreateFromTTFFile(entry.path().string())));
				}
			}
		}

		/* Load defaults from the editor resources */
		_defaultTexture = std::make_shared<TextureRes>(Utils::GetEditorResFolder() + "textures\\default.png");
		if (_defaultTexture->Load())
		{
			WYRD_CORE_ERROR("Unable to load default texture!");
		}

		/* Load default icons into the icon library */
		_iconLibrary.AddIconsFromFile(Utils::GetEditorResFolder() + "icons/common-icons.json");

		/* Subscribe to project events */
		ServiceManager::Get<EventService>(ServiceManager::Events)->Subscribe(Events::EventType::ProjectLoaded, EVENT_FUNC(ResourceService::OnProjectLoadedEvent));
		ServiceManager::Get<EventService>(ServiceManager::Events)->Subscribe(Events::EventType::AddFileEntry, EVENT_FUNC(ResourceService::OnAddFileEntryEvent));
		ServiceManager::Get<EventService>(ServiceManager::Events)->Subscribe(Events::EventType::DeleteFileEntry, EVENT_FUNC(ResourceService::OnDeleteFileEntryEvent));
		ServiceManager::Get<EventService>(ServiceManager::Events)->Subscribe(Events::EventType::RenameFileEntry, EVENT_FUNC(ResourceService::OnReloadFileEntryEvent));
		ServiceManager::Get<EventService>(ServiceManager::Events)->Subscribe(Events::EventType::ModifiedFileEntry, EVENT_FUNC(ResourceService::OnModifiedFileEntryEvent));
		ServiceManager::Get<EventService>(ServiceManager::Events)->Subscribe(Events::EventType::LoadAsset, EVENT_FUNC(ResourceService::OnLoadAssetEvent));
	}

	void ResourceService::OnDestroy() {}

	void ResourceService::OnUpdate()
	{
		if (!_loadableResources.empty())
		{
			_loadableResources.top()->Load();
			_loadableResources.pop();
		}
	}

	void ResourceService::AddResource(std::string& resourcePath, const UID uid)
	{
		if (CheckIgnored(resourcePath) == false)
		{
			/* determine the type of resource from the path */
			ResourceType resourceType = DetermineType(resourcePath);

			/* if we can't resolve a type, then we can skip the resource */
			if (resourceType != ResourceType::NONE)
			{
				/* create a new resource object */
				std::shared_ptr<Resource> resource = ResourceFactory::Create(resourceType, resourcePath);
				resource->SetResourceID(uid);

				/* add to the resource map */
				_resourceMap.insert(std::pair<UID, std::shared_ptr<Resource>>(resource->GetResourceID(), resource));

				/* load all resources for now */
				resource->Load();

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



	void ResourceService::BuildScripts()
	{
		/* ensure we compile on the main thread */
		if (Application::Get().GetMainThreadID() != std::this_thread::get_id())
		{
			/* swap an event to trigger the load on the main thread */
			ServiceManager::Get<EventService>(ServiceManager::Events)->Publish(Events::EventType::BuildBehaviourModel, std::make_unique<Events::BuildBehaviourModelArgs>());
		}
		else
		{
			ServiceManager::Get<SimulationService>(ServiceManager::Simulation)->CompileAll();

			// Resolve all the script resources to the scripted classes
			for (auto& [key, value] : _resourceMap)
			{
				auto downcastedPtr = std::dynamic_pointer_cast<ScriptRes>(value);
				if (downcastedPtr)
				{
					auto scriptedClass = ServiceManager::Get<SimulationService>(ServiceManager::Simulation)->GetClass(value->GetName());

					/* set the uid of the scripted class to match the resource */
					scriptedClass->SetUID(downcastedPtr->GetResourceID());
					downcastedPtr->Script = scriptedClass;
				}
			}
		}
	}

	bool ResourceService::CheckIgnored(const std::string& path)
	{
		std::string fileExtension = Utils::GetFileExtension(path);

		return (_ignoredExtensions.find(Utils::GetFileExtension(path)) != _ignoredExtensions.end());
	}

	ResourceType ResourceService::DetermineType(const std::string& path)
	{
		std::string& extension = Utils::GetFileExtension(path);

		std::map<std::string, ResourceType>::iterator it = _extensions.find(extension);

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
			else
			{
				Utils::HashFile(t);
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

				if (res.second->GetPath().rfind(oldPath, 0) == 0)
				{
					/* remove from the cached file list */
					CachedFiles.erase(res.second->GetPath());

					/* update the path */
					std::string newPath = a.filepath + res.second->GetPath().substr(oldPath.length());
					res.second->SetPath(newPath);

					/* remove from the cached file list */
					CachedFiles[newPath] = res.second->GetResourceID();
				}
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

	}

	void ResourceService::OnLoadAssetEvent(Events::EventArgs& args)
	{
		Events::LoadAssetArgs& a = (Events::LoadAssetArgs&)args;
		
		_loadableResources.push(a.resource);
	}
}
