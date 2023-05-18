/* core wyrd includes */
#include <wyrdpch.h>
#include <core/Log.h>

/* local includes */
#include "ResourceSetLoader.h"
#include "datamodels/resources/TextureRes.h"
#include "datamodels/resources/ShaderRes.h"
#include "datamodels/resources/MaterialRes.h"
#include "datamodels/resources/ModelRes.h"

/* external include */
#include <jsonxx.h>

namespace Wyrd::Editor
{
	ResourceSetLoader::Result ResourceSetLoader::Load(const std::filesystem::path& path)
	{
		ResourceSetLoader::Result result = FileNotFound;
		jsonxx::Object o;

		std::ifstream f(path);
		if (f.is_open() == true) {

			std::filesystem::path rootPath = path.parent_path();

			std::ostringstream ss;
			ss << f.rdbuf();

			if (o.parse(ss.str()) == true)
			{
				if (o.has<jsonxx::Array>("Textures"))
				{
					jsonxx::Array textureArray = o.get<jsonxx::Array>("Textures");

					for (size_t i = 0; i < textureArray.size(); i++)
					{
						jsonxx::Object textureObject = textureArray.get<jsonxx::Object>((int)i);

						std::string path = textureObject.get<jsonxx::String>("path");
						std::string resourceID = textureObject.get<jsonxx::String>("resourceID");

						std::shared_ptr<TextureRes> textureRes = std::make_shared<TextureRes>(rootPath / path, UID(resourceID));
						if (textureRes->Load() != 0)
						{
							WYRD_CORE_ERROR("Unable to load Texture Resource!");
						}
						else
						{
							WYRD_CORE_INFO("Texture Loaded : {0} - {1}", textureRes->GetName(), textureRes->GetResourceID().str());
						}
					}
				}

				if (o.has<jsonxx::Array>("Shaders"))
				{
					jsonxx::Array shaderArray = o.get<jsonxx::Array>("Shaders");

					for (size_t i = 0; i < shaderArray.size(); i++)
					{
						jsonxx::Object shaderObject = shaderArray.get<jsonxx::Object>((int)i);

						std::string path = shaderObject.get<jsonxx::String>("path");
						std::string resourceID = shaderObject.get<jsonxx::String>("resourceID");

						std::shared_ptr<ShaderRes> shaderRes = std::make_shared<ShaderRes>(rootPath / path, UID(resourceID));
						if (shaderRes->Load() != 0)
						{
							WYRD_CORE_ERROR("Unable to load Shader Resource!");
						}
						else
						{
							WYRD_CORE_INFO("Shader Loaded : {0} - {1}", shaderRes->GetName(), shaderRes->GetResourceID().str());
						}
					}
				}

				if (o.has<jsonxx::Array>("Materials"))
				{
					jsonxx::Array MaterialArray = o.get<jsonxx::Array>("Materials");

					for (size_t i = 0; i < MaterialArray.size(); i++)
					{
						jsonxx::Object materialObject = MaterialArray.get<jsonxx::Object>((int)i);

						std::string path = materialObject.get<jsonxx::String>("path");
						std::string resourceID = materialObject.get<jsonxx::String>("resourceID");

						std::shared_ptr<MaterialRes> materialRes = std::make_shared<MaterialRes>(rootPath / path, UID(resourceID));
						if (materialRes->Load() != 0)
						{
							WYRD_CORE_ERROR("Unable to load Material!");
						}
						else
						{
							WYRD_CORE_INFO("Material Loaded : {0} - {1}", materialRes->GetName(), materialRes->GetResourceID().str());
						}
					}
				}
				if (o.has<jsonxx::Array>("Meshes"))
				{
					jsonxx::Array meshArray = o.get<jsonxx::Array>("Meshes");
				
					for (size_t i = 0; i < meshArray.size(); i++)
					{
						jsonxx::Object meshObject = meshArray.get<jsonxx::Object>((int)i);

						std::string path = meshObject.get<jsonxx::String>("path");
						std::string resourceID = meshObject.get<jsonxx::String>("resourceID");

						std::shared_ptr<ModelRes> modelRes = std::make_shared<ModelRes>(rootPath / path, UID(resourceID));
						if (modelRes->Load() != 0)
						{
							WYRD_CORE_ERROR("Unable to load Mesh!");
						}
						else
						{
							WYRD_CORE_INFO("Mesh Loaded : {0} - {1}", modelRes->GetName(), modelRes->GetResourceID().str());
						}
					}
				}

				result = Success;
			}
			else
			{
				result = FileMalformed;
			}
		}

		return result;
	}
};