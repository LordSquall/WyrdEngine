#pragma once

/* core osiris includes */
#include <core/Log.h>

/* local includes */
#include "loaders/AssetCacheLoader.h"
#include "services/ServiceManager.h"
#include "services/ResourceService.h"
#include "datamodels/resources/SceneRes.h"

/* external includes */
#include <jsonxx.h>

namespace Osiris::Editor
{
	static void SaveScenes(jsonxx::Object& o, std::map<uint32_t, std::shared_ptr<Resource>> resources);
	static void SaveTextures(jsonxx::Object& o, std::map<uint32_t, std::shared_ptr<Resource>> resources);

	static void LoadScenes(jsonxx::Object& o);
	static void LoadTextures(jsonxx::Object& o);

	std::shared_ptr<ResourceService> _ResourceService = nullptr;

	AssetCacheLoader::Result AssetCacheLoader::Load(std::string path)
	{
		AssetCacheLoader::Result result = Success;
		jsonxx::Object o;

		std::ifstream f(path);

		if (f.is_open() == true) {
			std::ostringstream ss;
			ss << f.rdbuf();

			if (o.parse(ss.str()) == true)
			{
				LoadScenes(o);
				LoadTextures(o);
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
		_ResourceService = ServiceManager::Get<ResourceService>(ServiceManager::Resources);

		SaveScenes(o, _ResourceService->GetResourcesOfType(ResourceFactory::SCENE));
		SaveTextures(o, _ResourceService->GetResourcesOfType(ResourceFactory::TEXTURE));
		
		std::ofstream out(path);
		out << o.json();
		out.close();

		return result;
	}

	void SaveScenes(jsonxx::Object& o, std::map<uint32_t, std::shared_ptr<Resource>> resources)
	{
		jsonxx::Array scenesArray;
		for (auto& scene : _ResourceService->GetResourcesOfType(ResourceFactory::SCENE))
		{
			SceneRes* sceneRes = static_cast<SceneRes*>(&*scene.second);
			jsonxx::Object sceneObj;

			sceneObj << "name" << sceneRes->GetName();

			scenesArray << sceneObj;
		}

		o << "scenes" << scenesArray;
	}

	void SaveTextures(jsonxx::Object& o, std::map<uint32_t, std::shared_ptr<Resource>> resources)
	{
		/* Textures */
		jsonxx::Array texturesArray;
		for (auto& texture : _ResourceService->GetResourcesOfType(ResourceFactory::TEXTURE))
		{
			TextureRes* textureRes = static_cast<TextureRes*>(&*texture.second);
			jsonxx::Object textureObj;

			textureObj << "name" << textureRes->GetName();
			textureObj << "filepath" << textureRes->GetFilePath();

			texturesArray << textureObj;
		}

		o << "textures" << texturesArray;
	}

	void LoadScenes(jsonxx::Object& o)
	{
		jsonxx::Array scenesArray = o.get<jsonxx::Array>("scenes");
		
	}

	void LoadTextures(jsonxx::Object& o)
	{
		jsonxx::Array texturesArray = o.get<jsonxx::Array>("textures");

		for (int i = 0; i < texturesArray.size(); i++)
		{
			jsonxx::Object textureCacheEntry = texturesArray.get<jsonxx::Object>(i);

			jsonxx::String filePath;

			if (textureCacheEntry.has<jsonxx::String>("filepath"))
				filePath = textureCacheEntry.get<jsonxx::String>("filepath");
		}
	}
}
