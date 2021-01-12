#pragma once

/* core osiris includes */
#include <osrpch.h>
#include <core/renderer/Texture.h>

/* local includes */
#include "TextureRes.h"
#include "loaders/TextureLoader.h"
#include "services/ServiceManager.h"

/* external includes */
#include <core/Application.h>

namespace Osiris::Editor
{
	TextureRes::TextureRes(std::shared_ptr<Osiris::Texture> texture, std::string name) : Resource(name), _texture(texture), _loaded(false), _width(0u), _height(0u), _channels(0u), _data(0)
	{

	}

	TextureRes::TextureRes(const std::string& filepath, bool delayLoad) : Resource(Utils::GetFilename(filepath)), _filePath(filepath), _loaded(false), _width(0u), _height(0u), _channels(0u), _data(0)
	{

	}
	
	bool TextureRes::Load()
	{
		/* textures should always be loaded on the main thread */
		if (Application::Get().GetMainThreadID() != std::this_thread::get_id())
		{
			/* setup to use the default texture */
			_texture = ServiceManager::Get<ResourceService>(ServiceManager::Resources)->GetDefaultTexture()->GetTexture();

			/* swap an event to trigger the load on the main thread */
			ServiceManager::Get<EventService>(ServiceManager::Events)->Publish(Events::EventType::LoadAsset, std::make_shared<Events::LoadAssetArgs>(this));
		}
		else
		{
			/* load the data from file */
			TextureLoader::Load(_filePath, *this);

			/* create the core renderer resource and load */
			_texture = Osiris::Texture::Create(_data, _width, _height, _channels, Utils::GetFilename(_filePath, true) + "_Texture");
		}

		_loaded = true;

		return _loaded;
	}
}
