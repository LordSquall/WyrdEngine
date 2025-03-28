/* core wyrd includes */
#include <wyrdpch.h>
#include <core/Log.h>
#include <filesystem>

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
	ResourceSetLoader::Result ResourceSetLoader::Load(const std::filesystem::path& path, std::map<UID, std::shared_ptr<Resource>>& resourceMap)
	{
		ResourceSetLoader::Result result = FileNotFound;
		jsonxx::Array a;

		std::ifstream f(path);
		if (f.is_open() == true) {

			std::filesystem::path rootPath = path.parent_path();

			std::ostringstream ss;
			ss << f.rdbuf();

			if (a.parse(ss.str()) == true)
			{
				for (size_t i = 0; i < a.size(); i++)
				{
					jsonxx::Object resourceObj = a.get<jsonxx::Object>((int)i);
					
					/* retrieve the standard cache header fields */
					std::string resourceType = resourceObj.get<jsonxx::String>("type");
					std::string name = resourceObj.get<jsonxx::String>("name");
					UID resourceID = UID(resourceObj.get<jsonxx::String>("resourceID"));

					if (resourceType == "Texture")
					{
						//std::string path = resourceObj.get<jsonxx::String>("path");
						//std::shared_ptr<TextureRes> res = std::make_shared<TextureRes>(name, resourceID);
						//res->IsEditorOnly(true);
						//res->SetPath(rootPath / path);
						//if (res->Load((rootPath / path).string()) != 0)
						//{
						//	WYRD_CORE_ERROR("Unable to load Texture Resource!");
						//}
						//else
						//{
						//	WYRD_CORE_INFO("Texture Loaded : {0} - {1}", res->GetName(), res->GetResourceID().str());
						//	resourceMap.insert(std::pair<UID, std::shared_ptr<Resource>>(UID(resourceID), res));
						//}
					}
					else if (resourceType == "Mesh")
					{
						std::string path = resourceObj.get<jsonxx::String>("path");
						std::shared_ptr<ModelRes> res = std::make_shared<ModelRes>(name, resourceID);
						res->IsEditorOnly(true);
						res->SetPath(rootPath / path);
						if (res->Load((rootPath / path).string()) != 0)
						{
							WYRD_CORE_ERROR("Unable to load Model Resource!");
						}
						else
						{
							WYRD_CORE_INFO("Model Loaded : {0} - {1}", res->GetName(), res->GetResourceID().str());
							resourceMap.insert(std::pair<UID, std::shared_ptr<Resource>>(UID(resourceID), res));
						}
					}
					else if (resourceType == "ShaderStage")
					{
						std::string path = resourceObj.get<jsonxx::String>("path");
						std::shared_ptr<ShaderStageRes> res = std::make_shared<ShaderStageRes>(name, resourceID);
						res->IsEditorOnly(true);
						res->SetPath(rootPath / path);
						if (res->Load((rootPath / path).string()) != 0)
						{
							WYRD_CORE_ERROR("Unable to load ShaderStageRes Resource!");
						}
						else
						{
							WYRD_CORE_INFO("ShaderStageRes Loaded : {0} - {1}", res->GetName(), res->GetResourceID().str());
							resourceMap.insert(std::pair<UID, std::shared_ptr<Resource>>(UID(resourceID), res));
						}
					}
					else if (resourceType == "Shader")
					{
						jsonxx::Object data = resourceObj.get<jsonxx::Object>("data");
						std::shared_ptr<ShaderRes> res = std::make_shared<ShaderRes>(name, resourceID);
						res->IsEditorOnly(true);
						if (res->Load(data) != 0)
						{
							WYRD_CORE_ERROR("Unable to load ShaderRes Resource!");
						}
						else
						{
							WYRD_CORE_INFO("ShaderRes Loaded : {0} - {1}", res->GetName(), res->GetResourceID().str());
							resourceMap.insert(std::pair<UID, std::shared_ptr<Resource>>(UID(resourceID), res));
						}
					}
					else if (resourceType == "Material")
					{
						jsonxx::Object data = resourceObj.get<jsonxx::Object>("data");
						std::shared_ptr<MaterialRes> res = std::make_shared<MaterialRes>(name, resourceID);
						res->IsEditorOnly(true);
						if (res->Load(data) != 0)
						{
							WYRD_CORE_ERROR("Unable to load MaterialRes Resource!");
						}
						else
						{
							WYRD_CORE_INFO("MaterialRes Loaded : {0} - {1}", res->GetName(), res->GetResourceID().str());
							resourceMap.insert(std::pair<UID, std::shared_ptr<Resource>>(UID(resourceID), res));
						}
					}
					else
					{
						WYRD_CORE_WARN("Unrecognised Resource type in resourceset: {0} {1}", name, resourceType);
					}
				}

				/* trigger resolutions on all resources */
				for (auto& [k, v] : resourceMap)
				{
					v->ResolveReferences();
				}

				/* trigger build on all resources */
				for (auto& [k, v] : resourceMap)
				{
					v->Build();
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