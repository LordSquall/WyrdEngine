/* core wyrd includes */
#include <wyrdpch.h>
#include <core/renderer/Texture.h>

/* local includes */
#include "TextureRes.h"
#include "loaders/TextureLoader.h"
#include "services/ServiceManager.h"

/* external includes */
#include <core/Application.h>

namespace Wyrd::Editor
{
	TextureRes::TextureRes(const std::string& name, const UID& uid) : Resource(name, uid), _width(0u), _height(0u), _channels(0u), _data(0)
	{

	}
	
	Resource::IOResult TextureRes::Load(const std::string& filepath)
	{
		/* textures should always be loaded on the main thread */
		if (Application::Get().GetMainThreadID() != std::this_thread::get_id())
		{
			/* setup to use the default texture */
			_texture = ServiceManager::Get<ResourceService>()->GetDefaultTexture()->GetTexture();

			/* swap an event to trigger the load on the main thread */
			ServiceManager::Get<EventService>()->Publish(Events::EventType::LoadAsset, std::make_unique<Events::LoadAssetArgs>(this));
		}
		else
		{
			/* load the data from file */
			TextureLoader::Load(filepath, _width, _height, _channels, &_data);

			/* build the texture description structure */
			TextureDesc textureDesc;
			textureDesc.guid = _resourceID;
			textureDesc.name = filepath;
			textureDesc.data = _data;
			textureDesc.width = _width;
			textureDesc.height = _height;
			textureDesc.channels = _channels;
			textureDesc.description = _path.stem().string() + "_Texture";

			/* create the core renderer resource and load */
			_texture.reset(Wyrd::Texture::Create(textureDesc));

			/* register the texture with the core resource manager */
			Application::Get().GetResources().Textures[_resourceID] = _texture;

			/* create an icon set for the thumnail */
			_thumbnailIconSet.name = "thumbnail_iconset";
			_thumbnailIconSet.imagePath = filepath;
			_thumbnailIconSet.width = _width;
			_thumbnailIconSet.height = _height;
			_thumbnailIconSet.Texture.reset(this);

			_thumbnailIcon.width = _width;
			_thumbnailIcon.height = _height;
			_thumbnailIcon.x = 0;
			_thumbnailIcon.y = 0;
			_thumbnailIcon.uv[0] = { 0.0f, 0.0f };
			_thumbnailIcon.uv[1] = { 0.0f, 1.0f };
			_thumbnailIcon.uv[2] = { 1.0f, 1.0f };
			_thumbnailIcon.uv[3] = { 1.0f, 0.0f };
			_thumbnailIcon.name = "ThumbnailIcon";
			//thumbnailIcon->iconSet = std::make_shared<IconSet>();
			_thumbnailIcon.iconSet.reset(&_thumbnailIconSet);
			_thumbnailIconSet.Icons.push_back(std::make_shared<Icon>(_thumbnailIcon));

			_isLoaded = true;
		}

		return Success;
	}

	Resource::IOResult TextureRes::Load(const jsonxx::Object& obj)
	{
		return NotImplemented;
	}

	Resource::IOResult TextureRes::Save(const std::string& filepath) 
	{
		return NotImplemented;
	}

	const Icon& TextureRes::GetThumbnail()
	{
		return _thumbnailIcon;
	}

	const std::string TextureRes::GetTypeTag()
	{
		return RESOURCE_TAG_TEXTURE;
	}
}
