/* core wyrd includes */
#include <wyrdpch.h>
#include <core/renderer/Texture.h>

/* local includes */
#include "MaterialRes.h"
#include "loaders/TextureLoader.h"
#include "services/ServiceManager.h"

/* external includes */
#include <core/Application.h>

namespace Wyrd::Editor
{
	MaterialRes::MaterialRes(const std::filesystem::path& filepath) : Resource(filepath.stem().string(), filepath), _loaded(false), _width(0u), _height(0u), _channels(0u), _data(0)
	{

	}
	
	bool MaterialRes::Load()
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
			///* load the data from file */
			//TextureLoader::Load(_path, *this);
			//
			///* build the texture description structure */
			//TextureDesc textureDesc;
			//textureDesc.data = _data;
			//textureDesc.width = _width;
			//textureDesc.height = _height;
			//textureDesc.channels = _channels;
			//textureDesc.description = _path.stem().string() + "_Texture";
			//
			///* create the core renderer resource and load */
			//_texture.reset(Wyrd::Texture::Create(textureDesc));
			//
			///* in the editor project, we want to the core assets uid to match the resource to allow easy decoupled linkage */
			//_texture->SetUID(_resourceID);
			//
			///* register the texture with the core resource manager */
			//Application::Get().GetResources().Textures[_resourceID] = _texture;
			//
			//_loaded = true;
		}


		return _loaded;
	}
}
