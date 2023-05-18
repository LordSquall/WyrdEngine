/* core wyrd includes */
#include <wyrdpch.h>

/* local includes */
#include "ShaderLoader.h"

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
				/* name */
				name = o.get<jsonxx::String>("name", "default");

				/* vertex source */
				std::string vsPath = o.get<jsonxx::String>("vertexSrc", "default");

				/* vertex source */
				std::string fsPath = o.get<jsonxx::String>("fragementSrc", "default");

				std::ifstream vertexStream(path.parent_path() / vsPath);
				vs.append((std::istreambuf_iterator<char>(vertexStream)), std::istreambuf_iterator<char>());

				std::ifstream fragmentStream(path.parent_path() / fsPath);
				fs.append((std::istreambuf_iterator<char>(fragmentStream)), std::istreambuf_iterator<char>());

				result = Success;
			}
			else
			{
				result = FileMalformed;
			}
		}

		return result;
	}

	ShaderLoader::Result ShaderLoader::Save(const std::filesystem::path& path)
	{
		// TODO
		return Success;
	}

}