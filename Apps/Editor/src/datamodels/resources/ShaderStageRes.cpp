/* core wyrd includes */
#include <wyrdpch.h>
#include <core/renderer/Shader.h>

/* local includes */
#include "ShaderStageRes.h"
#include "services/ServiceManager.h"
#include "loaders/ShaderLoader.h"

/* external includes */
#include <core/Application.h>
#include <imgui.h>

namespace Wyrd::Editor
{
	ShaderStageRes::ShaderStageRes(const std::string& name, const UID& uid) : Resource(name, uid)
	{

	}
	
	Resource::IOResult ShaderStageRes::Load(const std::string& filepath)
	{
		/* load the shader from file */
		std::ifstream fileStream(_path);
		_source.clear();
		_source.append((std::istreambuf_iterator<char>(fileStream)), std::istreambuf_iterator<char>());

		_isLoaded = true;

		return Success;
	}

	Resource::IOResult ShaderStageRes::Load(const jsonxx::Object& object)
	{
		return NotImplemented;
	}

	Resource::IOResult ShaderStageRes::Save(const std::string& filepath) {
		return NotImplemented;
	}

	const Icon& ShaderStageRes::GetThumbnail()
	{
		return ServiceManager::Get<ResourceService>()->RetrieveIcon("common", "assets_shaderstage_fs");
	}

	const std::string ShaderStageRes::GetTypeTag()
	{
		return RESOURCE_TAG_SHADERSTAGE;
	}

	void ShaderStageRes::DrawProperties()
	{
		ImGui::Text(_source.c_str());
	}
}
