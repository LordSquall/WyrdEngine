/* core wyrd includes */
#include <wyrdpch.h>

/* local includes */
#include "ShaderLoader.h"
#include "serial/TypeSerialisers.h"
#include "serial/ComponentSerialiserFactory.h"
#include "serial/CustomComponentSerialisationFunctions.h"

/* external include */
#include <jsonxx.h>

namespace Wyrd::Editor
{;
	ShaderLoader::Result ShaderLoader::Load(const std::filesystem::path& path, std::string& name, std::string& vs, std::string& fs)
	{
		ShaderLoader::Result result = FileNotFound;
		jsonxx::Object o;
		
		std::ifstream f(path);

		if (f.is_open() == true) {
			std::ostringstream ss;
			ss << f.rdbuf();

			if (o.parse(ss.str()) == true)
			{
				result = Load(o, name, vs, fs);
			}
			else
			{
				result = FileMalformed;
			}
		}

		return result;
	}

	ShaderLoader::Result ShaderLoader::Load(const jsonxx::Object& obj, std::string& name, std::string& vs, std::string& fs)
	{
		/* name */
		name = obj.get<jsonxx::String>("name", "default");

		std::string vsData;
		std::string fsData;

		/* vertex source */
		if (obj.has<jsonxx::String>("vertexSrc") && obj.has<jsonxx::String>("fragementSrc"))
		{
			vs = obj.get<jsonxx::String>("vertexSrc", "default");
			fs = obj.get<jsonxx::String>("vertexSrc", "default");
		}
		else if (obj.has<jsonxx::String>("vertexStageID") && obj.has<jsonxx::String>("fragmentStageID"))
		{
			vs = obj.get<jsonxx::String>("vertexStageID", "default");
			fs = obj.get<jsonxx::String>("fragmentStageID", "default");
		}

		return Success;
	}

	ShaderLoader::Result ShaderLoader::Save(const std::filesystem::path& path, ShaderRes* shaderRes)
	{
		/* construct into a json file */
		jsonxx::Object o;

		/* name */
		o << "name" << shaderRes->GetName();

		/* vertex stage id */
		if (shaderRes->GetVertexStage() != nullptr)
		{
			o << "vertexStageID" << shaderRes->GetVertexStage()->GetResourceID();
		}
		else
		{
			o << "vertexStageID" << "unbound";
		}

		/* fragment stage id */
		if (shaderRes->GetFragmentStage() != nullptr)
		{
			o << "fragmentStageID" << shaderRes->GetFragmentStage()->GetResourceID();
		}
		else
		{
			o << "fragmentStageID" << "unbound";
		}

		std::ofstream out(path);
		out << o.json();
		out.close();
		return Success;
	}

}