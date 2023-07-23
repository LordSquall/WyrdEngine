/* core wyrd includes */
#include <wyrdpch.h>
#include <core/renderer/Shader.h>

/* local includes */
#include "ShaderRes.h"
#include "services/ServiceManager.h"
#include "loaders/ShaderLoader.h"

/* external includes */
#include <core/Application.h>

namespace Wyrd::Editor
{
	ShaderRes::ShaderRes(const std::filesystem::path& filepath, const UID& uid) : Resource(filepath.stem().string(), filepath, uid)
	{

	}
	
	int ShaderRes::Load()
	{
		/* load the shader from file */
		std::string name;
		std::string vs;
		std::string fs;
		ShaderLoader::Load(_path, name, vs, fs);

		/* build shader description */
		ShaderDesc shaderDesc;
		shaderDesc.resource.guid = UIDUtils::Create();
		shaderDesc.resource.name = name;

		/* create the core renderer resource and load */
		_shader.reset(Shader::Create(shaderDesc));
		_shader->Build(vs, fs);

		/* register the texture with the core resource manager */
		Application::Get().GetResources().Shaders[name] = _shader;

		_isLoaded = true;
		return 0;
	}
}
