/* core wyrd includes */
#include <wyrdpch.h>

/* local includes */
#include "ProjectLoader.h"

/* external include */
#include <jsonxx.h>

namespace Wyrd::Editor
{;
	ProjectLoader::Result ProjectLoader::Load(const std::filesystem::path& path, Project& project)
	{
		ProjectLoader::Result result = FileNotFound;
		jsonxx::Object o;
		
		std::ifstream f(path);

		if (f.is_open() == true) {
			std::ostringstream ss;
			ss << f.rdbuf();

			if (o.parse(ss.str()) == true)
			{
				if (project.FromJson(o))
					result = Success;
			}
			else
			{
				result = FileMalformed;
			}
		}

		return result;
	}

	ProjectLoader::Result ProjectLoader::Save(const std::filesystem::path& path, Project& project)
	{
		ProjectLoader::Result result = Success;
		jsonxx::Object o = project.ToJson();

		std::ofstream out(path);
		out << o.json();
		out.close();

		return result;
	}

}