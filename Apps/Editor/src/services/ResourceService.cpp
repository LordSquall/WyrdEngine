#pragma once

#include "osrpch.h"
#include "ResourceService.h"
#include "services/ServiceManager.h"
#include "loaders/TextureLoader.h"
#include "datamodels/resources/TextureRes.h"


namespace Osiris::Editor
{
	void ResourceService::OnCreate()
	{
		/* Register the file extensions for each of the resource types */
		_extensions.insert(std::pair<std::string, Type>(".png", TEXTURE));
		_extensions.insert(std::pair<std::string, Type>(".jpg", TEXTURE));
		_extensions.insert(std::pair<std::string, Type>(".bmp", TEXTURE));
		_extensions.insert(std::pair<std::string, Type>(".vs", SHADER));
		_extensions.insert(std::pair<std::string, Type>(".fs", SHADER));
		_extensions.insert(std::pair<std::string, Type>(".scene", SCENE));
		_extensions.insert(std::pair<std::string, Type>(".lua", SCRIPT));

		/* Load defaults from the editor resources */
		_defaultTexture = std::make_shared<TextureRes>(Utils::GetEditorResFolder() + "\\res\\textures\\default.png");
		//_defaultScene = std::make_shared<SceneRes>(Utils::GetEditorResFolder() + "\\res\\scenes\\default.scene");

		/* Load default icons into the icon library */
		_iconLibrary.AddIconsFromFile(Utils::GetEditorResFolder() + "/res/icons/common-icons.json");

		/* Subscribe to project events */
		ServiceManager::Get<EventService>(ServiceManager::Events)->Subscribe(Events::EventType::ProjectLoaded, EVENT_FUNC(ResourceService::OnProjectLoadedEvent));
		ServiceManager::Get<EventService>(ServiceManager::Events)->Subscribe(Events::EventType::AddResource, EVENT_FUNC(ResourceService::OnAddResourceEvent));
		ServiceManager::Get<EventService>(ServiceManager::Events)->Subscribe(Events::EventType::DeleteResource, EVENT_FUNC(ResourceService::OnDeleteResourceEvent));
		ServiceManager::Get<EventService>(ServiceManager::Events)->Subscribe(Events::EventType::ReloadResource, EVENT_FUNC(ResourceService::OnReloadResourceEvent));
	}

	void ResourceService::OnDestroy() {}


	void ResourceService::AddResource(std::string& resourcePath, ResourceService::Type type)
	{
		Type resourceType = DetermineType(resourcePath);

		switch (resourceType)
		{
		case Type::TEXTURE:
			{
				std::shared_ptr<TextureRes> textureResource = std::make_shared<TextureRes>(resourcePath);
				_textureResources.insert(std::pair<uint32_t, std::shared_ptr<TextureRes>>(textureResource->GetUID(), textureResource));

				OSR_CORE_INFO("Asset Texture Added: [{0}] - {1}", textureResource->GetUID(), textureResource->GetName());
			}
			break;
		case Type::SCENE:
			{
				std::shared_ptr<SceneRes> sceneResource = std::make_shared<SceneRes>(resourcePath);
				_sceneResources.insert(std::pair<uint32_t, std::shared_ptr<SceneRes>>(sceneResource->GetUID(), sceneResource));

				OSR_CORE_INFO("Asset Scene Added: [{0}] - {1}", sceneResource->GetUID(), sceneResource->GetName());
			}
			break;
		case Type::SCRIPT:
			{
				std::shared_ptr<ScriptRes> scriptResource = std::make_shared<ScriptRes>(resourcePath);
				_scriptResources.insert(std::pair<uint32_t, std::shared_ptr<ScriptRes>>(scriptResource->GetUID(), scriptResource));

				OSR_CORE_INFO("Asset Script Added: [{0}] - {1}", scriptResource->GetUID(), scriptResource->GetName());
			}
			break;
		default:
			break;
		}
	}

	void ResourceService::ReloadResource(std::string& resourcePath)
	{
		Type resourceType = DetermineType(resourcePath);

		switch (resourceType)
		{
		case Type::TEXTURE:
		{
			std::shared_ptr<TextureRes> textureResource = GetTextureByName(Utils::GetFilename(resourcePath));
			textureResource->Reload();
		}
		break;
		case Type::SCRIPT:
		{
			std::shared_ptr<ScriptRes> scriptResource = GetScriptByName(Utils::GetFilename(resourcePath));
			scriptResource->Reload();
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
			std::shared_ptr<TextureRes> textureResource = GetTextureByName(Utils::GetFilename(resourcePath));
			textureResource->Unload();

			_textureResources.erase(textureResource->GetUID());

			OSR_CORE_INFO("Asset Texture Deleted: [{0}] - {1}", textureResource->GetUID(), textureResource->GetName());
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
	std::shared_ptr<TextureRes> ResourceService::GetTextureByName(const std::string& name)
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

	std::shared_ptr<TextureRes> ResourceService::GetTextureByUID(const uint32_t uid)
	{
		auto it = _textureResources.find(uid);

		if (it != _textureResources.end())
		{
			return _textureResources[uid];
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
		_textureResources.clear();
		_sceneResources.clear();
		_scriptResources.clear();

		auto files = Utils::GetFileList(Utils::GetAssetFolder(), true, true);

		for (auto t : files)
		{
			AddResource(t, TEXTURE);
		}
	}

	void ResourceService::OnAddResourceEvent(Events::EventArgs& args)
	{
		Events::AddResourceArgs& a = (Events::AddResourceArgs&)args;
	
		AddResource(a.filepath, TEXTURE);
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
