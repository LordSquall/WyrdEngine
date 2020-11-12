#pragma once

#include "osrpch.h"
#include "core/Log.h"
#include "ResourceService.h"
#include "services/ServiceManager.h"
#include "loaders/TextureLoader.h"
#include "datamodels/resources/TextureRes.h"


namespace Osiris::Editor
{
	void ResourceService::OnCreate()
	{
		/* Register the ignored file extensions */
		_ignoredExtensions.insert(".dll");
		_ignoredExtensions.insert(".mdb");

		/* Register the file extensions for each of the resource types */
		_extensions.insert(std::pair<std::string, Type>(".png", TEXTURE));
		_extensions.insert(std::pair<std::string, Type>(".jpg", TEXTURE));
		_extensions.insert(std::pair<std::string, Type>(".bmp", TEXTURE));
		_extensions.insert(std::pair<std::string, Type>(".vs", SHADER));
		_extensions.insert(std::pair<std::string, Type>(".fs", SHADER));
		_extensions.insert(std::pair<std::string, Type>(".scene", SCENE));
		_extensions.insert(std::pair<std::string, Type>(".cs", SCRIPT));

		/* Add resources from the osiris core to make then accessible with in the editor */
		for(auto& textures : Resources::Get().Textures)
		{
			std::shared_ptr<TextureRes> textureResource = std::make_shared<TextureRes>(textures.second, "default");
			_textureResources.insert(std::pair<uint32_t, std::shared_ptr<TextureRes>>(textureResource->GetResourceID(), textureResource));

			OSR_CORE_INFO("Asset Texture Added: [{0}] - {1}", textureResource->GetResourceID(), textureResource->GetName());
		}

		/* Load defaults from the editor resources */
		_defaultTexture = std::make_shared<TextureRes>(Utils::GetEditorResFolder() + "\\res\\textures\\default.png");

		/* Load default icons into the icon library */
		_iconLibrary.AddIconsFromFile(Utils::GetEditorResFolder() + "/res/icons/common-icons.json");

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
			Type resourceType = DetermineType(resourcePath);

			switch (resourceType)
			{
			case Type::TEXTURE:
			{
				std::shared_ptr<TextureRes> textureResource = std::make_shared<TextureRes>(resourcePath);
				_textureResources.insert(std::pair<uint32_t, std::shared_ptr<TextureRes>>(textureResource->GetResourceID(), textureResource));

				OSR_CORE_INFO("Asset Texture Added: [{0}] - {1}", textureResource->GetResourceID(), textureResource->GetName());
			}
			break;
			case Type::SCENE:
			{
				std::shared_ptr<SceneRes> sceneResource = std::make_shared<SceneRes>(resourcePath);
				_sceneResources.insert(std::pair<uint32_t, std::shared_ptr<SceneRes>>(sceneResource->GetResourceID(), sceneResource));

				OSR_CORE_INFO("Asset Scene Added: [{0}] - {1}", sceneResource->GetResourceID(), sceneResource->GetName());
			}
			break;
			case Type::SCRIPT:
			{
				std::shared_ptr<ScriptRes> scriptResource = std::make_shared<ScriptRes>(resourcePath);
				_scriptResources.insert(std::pair<uint32_t, std::shared_ptr<ScriptRes>>(scriptResource->GetResourceID(), scriptResource));

				OSR_CORE_INFO("Asset Script Added: [{0}]", scriptResource->GetResourceID());
			}
			break;
			default:
				break;
			}
		}
		else
		{
			//OSR_CORE_INFO("Skipping Asset file: [{0}] - As extension is on the ignored list!", resourcePath);
		}
	}

	void ResourceService::ReloadResource(std::string& resourcePath)
	{
		Type resourceType = DetermineType(resourcePath);

		switch (resourceType)
		{
		case Type::TEXTURE:
		{
			std::shared_ptr<TextureRes> textureResource = GetTextureResourceByName(Utils::GetFilename(resourcePath));
			textureResource->Reload();
		}
		break;
		case Type::SCRIPT:
		{
			std::shared_ptr<ScriptRes> scriptResource = GetScriptByName(Utils::GetFilename(resourcePath));
			scriptResource->Reload();

			BuildScripts();
		}
		break;
		case Type::SCENE:
		default:
			break;
		}

	}

	void ResourceService::DeleteResource(std::string& resourcePath)
	{
		Type resourceType = DetermineType(resourcePath);

		switch (resourceType)
		{
		case Type::TEXTURE:
		{
			std::shared_ptr<TextureRes> textureResource = GetTextureResourceByName(Utils::GetFilename(resourcePath));
			textureResource->Unload();

			_textureResources.erase(textureResource->GetResourceID());

			OSR_CORE_INFO("Asset Texture Deleted: [{0}] - {1}", textureResource->GetResourceID(), textureResource->GetName());
		}
		break;
		case Type::SCENE:
		{
			OSR_CORE_INFO("Asset Scene Delete - not yet implemented");
		}
		break;
		case Type::SCRIPT:
		{
			OSR_CORE_INFO("Asset Script Delete - not yet implemented");
		}
		break;
		default:
			break;
		}

	}

	/* Texture Functions */
	std::shared_ptr<TextureRes> ResourceService::GetTextureResourceByName(const std::string& name)
	{
		for (auto const& [key, val] : _textureResources)
		{
			if (val->GetName().compare(name) == 0)
			{
				return val;
			}
		}
		return _defaultTexture;
	}

	std::shared_ptr<TextureRes> ResourceService::GetTextureResourceByID(const uint32_t resourceId)
	{
		auto it = _textureResources.find(resourceId);

		if (it != _textureResources.end())
		{
			return _textureResources[resourceId];
		}
		return nullptr;
	}

	std::shared_ptr<TextureRes> ResourceService::GetTextureResourceByNativeID(const uint32_t nativeId)
	{
		for(auto& resource : _textureResources)
		{
			if (resource.second->GetTexture()->GetUID() == nativeId)
				return resource.second;
		}
		
		return nullptr;
	}

	/* Scene Functions */
	std::shared_ptr<SceneRes> ResourceService::GetSceneByName(const std::string& name)
	{
		for (auto const& [key, val] : _sceneResources)
		{
			if (val->GetName().compare(name) == 0)
			{
				return val;
			}
		}
		return nullptr;
	}

	std::shared_ptr<SceneRes> ResourceService::GetSceneByUID(const uint32_t uid)
	{
		auto it = _sceneResources.find(uid);

		if (it != _sceneResources.end())
		{
			return _sceneResources[uid];
		}
		return nullptr;
	}

	/* Script Functions */
	std::shared_ptr<ScriptRes> ResourceService::GetScriptByName(const std::string& name)
	{
		if (name == "")
			return nullptr;

		for (auto const& [key, val] : _scriptResources)
		{
			if (val->GetName().compare(name) == 0)
			{
				return val;
			}
		}
		return nullptr;
	}

	std::shared_ptr<ScriptRes> ResourceService::GetScriptByUID(const uint32_t uid)
	{
		auto it = _scriptResources.find(uid);

		if (it != _scriptResources.end())
		{
			return _scriptResources[uid];
		}
		return nullptr;
	}

	void ResourceService::BuildScripts()
	{
		ServiceManager::Get<SimulationService>(ServiceManager::Simulation)->CompileAll();

		// Resolve all the script resources to the scripted classes
		for (auto& [key, value] : _scriptResources)
		{
			value->Script = ServiceManager::Get<SimulationService>(ServiceManager::Simulation)->GetClass(value->GetName());
		}
	}

	bool ResourceService::CheckIgnored(const std::string& path)
	{
		std::string fileExtension = Utils::GetFileExtension(path);

		return (_ignoredExtensions.find(Utils::GetFileExtension(path)) != _ignoredExtensions.end());
	}

	ResourceService::Type ResourceService::DetermineType(const std::string& path)
	{
		std::string& extension = Utils::GetFileExtension(path);

		std::map<std::string, Type>::iterator it = _extensions.find(extension);

		if (it != _extensions.end())
		{
			return it->second;
		}
		else
		{
			return NONE;
		}
	}

	void ResourceService::OnProjectLoadedEvent(Events::EventArgs& args)
	{
		Events::ProjectLoadedArgs& a = (Events::ProjectLoadedArgs&)args;

		/* Clear all resources */
		//_textureResources.clear();
		//_sceneResources.clear();
		//_scriptResources.clear();

		auto files = Utils::GetFileList(Utils::GetAssetFolder(), true, true);

		for (auto t : files)
		{
			AddResource(t);
		}

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
