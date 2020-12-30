#pragma once

/* core osiris includes */
#include <osrpch.h>
#include <core/Log.h>
#include <core/Resources.h>

/* local includes */
#include "ResourceService.h"
#include "services/ServiceManager.h"
#include "loaders/AssetCacheLoader.h"
#include "loaders/TextureLoader.h"
#include "datamodels/resources/TextureRes.h"

namespace Osiris::Editor
{
	void ResourceService::OnCreate()
	{
		/* Register the ignored file extensions */
		_ignoredExtensions.insert(".dll");
		_ignoredExtensions.insert(".mdb");

		/* Register the raw file extensions. These will be directly importable to the editor and create a new resource cache file */
		_extensions.insert(std::pair<std::string, ResourceFactory::Type>(".png", ResourceFactory::TEXTURE));
		_extensions.insert(std::pair<std::string, ResourceFactory::Type>(".jpg", ResourceFactory::TEXTURE));
		_extensions.insert(std::pair<std::string, ResourceFactory::Type>(".bmp", ResourceFactory::TEXTURE));
		_extensions.insert(std::pair<std::string, ResourceFactory::Type>(".vs", ResourceFactory::SHADER));
		_extensions.insert(std::pair<std::string, ResourceFactory::Type>(".fs", ResourceFactory::SHADER));
		_extensions.insert(std::pair<std::string, ResourceFactory::Type>(".cs", ResourceFactory::SCRIPT));

		/* Register the cache file extensions for each of the imported resource types */
		_extensions.insert(std::pair<std::string, ResourceFactory::Type>(".texture", ResourceFactory::TEXTURE));
		_extensions.insert(std::pair<std::string, ResourceFactory::Type>(".shader", ResourceFactory::SHADER));
		_extensions.insert(std::pair<std::string, ResourceFactory::Type>(".scene", ResourceFactory::SCENE));
		_extensions.insert(std::pair<std::string, ResourceFactory::Type>(".script", ResourceFactory::SCRIPT));

		/* Add resources from the osiris core to make then accessible with in the editor */
		for(auto& textures : Resources::Get().Textures)
		{
			std::shared_ptr<TextureRes> textureResource = std::make_shared<TextureRes>(textures.second, "default");
			//_textureResources.insert(std::pair<uint32_t, std::shared_ptr<TextureRes>>(textureResource->GetResourceID(), textureResource));

			OSR_CORE_INFO("Asset Texture Added: [{0}] - {1}", textureResource->GetResourceID(), textureResource->GetName());
		}

		/* Load defaults from the editor resources */
		_defaultTexture = std::make_shared<TextureRes>(Utils::GetEditorResFolder() + "textures\\default.png");

		/* Load default icons into the icon library */
		_iconLibrary.AddIconsFromFile(Utils::GetEditorResFolder() + "icons/common-icons.json");

		/* Subscribe to project events */
		ServiceManager::Get<EventService>(ServiceManager::Events)->Subscribe(Events::EventType::ProjectLoaded, EVENT_FUNC(ResourceService::OnProjectLoadedEvent));
		ServiceManager::Get<EventService>(ServiceManager::Events)->Subscribe(Events::EventType::AddResource, EVENT_FUNC(ResourceService::OnAddResourceEvent));
		ServiceManager::Get<EventService>(ServiceManager::Events)->Subscribe(Events::EventType::DeleteResource, EVENT_FUNC(ResourceService::OnDeleteResourceEvent));
		ServiceManager::Get<EventService>(ServiceManager::Events)->Subscribe(Events::EventType::ReloadResource, EVENT_FUNC(ResourceService::OnReloadResourceEvent));
	}

	void ResourceService::OnDestroy() {}


	void ResourceService::AddResource(std::string& resourcePath)
	{
		if (CheckIgnored(resourcePath) == false)
		{
			/* determine the type of resource from the path */
			ResourceFactory::Type resourceType = DetermineType(resourcePath);

			/* create a new resource object */
			std::shared_ptr<Resource> resource = ResourceFactory::Create(resourceType, resourcePath);

			/* add to the resource map */
			_resourceMap[resourceType].insert(std::pair<uint32_t, std::shared_ptr<Resource>>(resource->GetResourceID(), resource));
		}
		else
		{
			//OSR_CORE_INFO("Skipping Asset file: [{0}] - As extension is on the ignored list!", resourcePath);
		}
	}

	void ResourceService::ReloadResource(std::string& resourcePath)
	{

	}

	void ResourceService::DeleteResource(std::string& resourcePath)
	{

	}

	std::map<uint32_t, std::shared_ptr<Resource>> ResourceService::GetResourcesOfType(ResourceFactory::Type type)
	{
		if (_resourceMap.count(type))
			return _resourceMap[type];

		return std::map<uint32_t, std::shared_ptr<Resource>>();
	}

	void ResourceService::BuildScripts()
	{
		ServiceManager::Get<SimulationService>(ServiceManager::Simulation)->CompileAll();

		// Resolve all the script resources to the scripted classes
		/*for (auto& [key, value] : _scriptResources)
		{
			value->Script = ServiceManager::Get<SimulationService>(ServiceManager::Simulation)->GetClass(value->GetName());
		}*/
	}

	bool ResourceService::CheckIgnored(const std::string& path)
	{
		std::string fileExtension = Utils::GetFileExtension(path);

		return (_ignoredExtensions.find(Utils::GetFileExtension(path)) != _ignoredExtensions.end());
	}

	ResourceFactory::Type ResourceService::DetermineType(const std::string& path)
	{
		std::string& extension = Utils::GetFileExtension(path);

		std::map<std::string, ResourceFactory::Type>::iterator it = _extensions.find(extension);

		if (it != _extensions.end())
		{
			return it->second;
		}
		else
		{
			return ResourceFactory::NONE;
		}
	}


	bool ResourceService::LoadAssetCache(const std::string& filePath)
	{
		if (!std::filesystem::exists(filePath))
		{
			OSR_INFO("Unable to find Asset cache file.");
			return true;
		}

		AssetCacheLoader::Load(Utils::GetCacheFolder() + "/assets.cache");

		return true;
	}

	void ResourceService::OnProjectLoadedEvent(Events::EventArgs& args)
	{
		Events::ProjectLoadedArgs& a = (Events::ProjectLoadedArgs&)args;

		/* load the asset cache */
		LoadAssetCache(Utils::GetCacheFolder() + "/assets.cache");


		/* Clear all resources */
		//_textureResources.clear();
		//_sceneResources.clear();
		//_scriptResources.clear();

		auto files = Utils::GetFileList(Utils::GetAssetFolder(), true, true);

		for (auto t : files)
		{
			AddResource(t);
		}

		/* Save the cache file */
		AssetCacheLoader::Save(Utils::GetCacheFolder() + "/assets.cache");

		/* Once all the resources are loaded, now we can compile the all the scripts */
		BuildScripts();
	}

	void ResourceService::OnAddResourceEvent(Events::EventArgs& args)
	{
		Events::AddResourceArgs& a = (Events::AddResourceArgs&)args;
	
		AddResource(a.filepath);
	}

	void ResourceService::OnDeleteResourceEvent(Events::EventArgs& args)
	{
		Events::DeleteResourceArgs& a = (Events::DeleteResourceArgs&)args;

		DeleteResource(a.filepath);
	}

	void ResourceService::OnReloadResourceEvent(Events::EventArgs& args)
	{
		Events::ReloadResourceArgs& a = (Events::ReloadResourceArgs&)args;

		ReloadResource(a.filepath);
	}
}
