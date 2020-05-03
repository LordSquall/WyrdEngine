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

		/* Load defaults from the editor resources */
		_defaultTexture = std::make_shared<TextureRes>(Utils::GetEditorResFolder() + "\\res\\textures\\default.png");
		//_defaultScene = std::make_shared<SceneRes>(Utils::GetEditorResFolder() + "\\res\\scenes\\default.scene");

		/* Subscribe to project lifecycle events */
		ServiceManager::Get<EventService>(ServiceManager::Events)->Subscribe(Events::EventType::ProjectLoaded, [this](Events::EventArgs& args) {
			Events::ProjectLoadedArgs& a = (Events::ProjectLoadedArgs&)args;

			/* Clear all resources */
			_textureResources.clear();
			_sceneResources.clear();

			auto files = Utils::GetFileList(Utils::GetAssetFolder(), true, true);

			for(auto t : files)
			{
				OSR_CORE_TRACE("Asset File: " + t);

				AddResource(t, TEXTURE);
			}

		});
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
			}
			break;
		case Type::SCENE:
			{
				std::shared_ptr<SceneRes> sceneResource = std::make_shared<SceneRes>(resourcePath);
				_sceneResources.insert(std::pair<uint32_t, std::shared_ptr<SceneRes>>(sceneResource->GetUID(), sceneResource));
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

	ResourceService::Type ResourceService::DetermineType(std::string& path)
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
}
