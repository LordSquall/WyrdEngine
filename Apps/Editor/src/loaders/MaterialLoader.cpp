/* core wyrd includes */
#include <wyrdpch.h>
#include <core/Application.h>
#include <core/Resources.h>

/* local includes */
#include "MaterialLoader.h"

/* external include */
#include <jsonxx.h>

namespace Wyrd::Editor
{
	MaterialLoader::Result MaterialLoader::Load(const std::filesystem::path& path, std::string& name, Material* material)
	{
		MaterialLoader::Result result = FileNotFound;
		jsonxx::Object o;
		
		std::ifstream f(path);

		if (f.is_open() == true) {
			std::ostringstream ss;
			ss << f.rdbuf();

			if (o.parse(ss.str()) == true)
			{
				/* name */
				name = o.get<jsonxx::String>("name", "default");

				/* export settings */
				if (o.has<jsonxx::Object>("shader"))
				{
					jsonxx::Object shaderObject = o.get<jsonxx::Object>("shader");

					/* shader name */
					std::string shaderName = shaderObject.get<jsonxx::String>("name", "default");

					/* retrieve the shader from resources */
					material->SetShaderName(shaderName);
					material->SetShader(Application::Get().GetResources().Shaders[shaderName]);
					material->SetName(name);

					/* check for shader bindings */
					if (shaderObject.has<jsonxx::Array>("inputs"))
					{
						jsonxx::Array inputsArray = shaderObject.get<jsonxx::Array>("inputs");

						for (size_t i = 0; i < inputsArray.size(); i++)
						{
							jsonxx::Object inputObject = inputsArray.get<jsonxx::Object>((int)i);

							std::string name = inputObject.get<jsonxx::String>("name");
							std::string type = inputObject.get<jsonxx::String>("type");
							std::string binding = inputObject.get<jsonxx::String>("binding");

							material->AddInputBinding(name, type, binding);
						}

					}

					result = Success;
				}
			}
			else
			{
				result = FileMalformed;
			}
		}

		return result;
	}

	MaterialLoader::Result MaterialLoader::Save(const std::filesystem::path& path)
	{
		// TODO
		return Success;
	}

}