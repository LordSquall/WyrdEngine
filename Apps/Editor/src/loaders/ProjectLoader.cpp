#pragma once

/* core osiris includes */
#include <osrpch.h>

/* local includes */
#include "ProjectLoader.h"

/* external include */
#include <jsonxx.h>

namespace Osiris::Editor
{;
	ProjectLoader::Result ProjectLoader::Load(std::string path, Project& project)
	{
		ProjectLoader::Result result = Success;
		jsonxx::Object o;
		
		std::ifstream f(path);

		if (f.is_open() == true) {
			std::ostringstream ss;
			ss << f.rdbuf();

			if (o.parse(ss.str()) == true)
			{
				project.FromJson(o);
			}
			else
			{
				result = FileMalformed;
			}
		}

		return result;
	}

	ProjectLoader::Result ProjectLoader::Save(std::string path, Project& project)
	{
		ProjectLoader::Result result = Success;
		jsonxx::Object o = project.ToJson();

		std::ofstream out(path);
		out << o.json();
		out.close();

		return result;
	}

}