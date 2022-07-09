#include "wyrdpch.h"

/* Local includes */
#include "RecentProjects.h"

namespace Wyrd::Editor
{
	RecentProjects::RecentProjects() 
	{ 
		Populate();
	}

	void RecentProjects::Populate()
	{
		/* Load recent projects */
		jsonxx::Object o;
		std::ifstream f("recentProject.json");
		if (f.is_open() == true) {
			std::ostringstream ss;
			ss << f.rdbuf();

			if (o.parse(ss.str()) == true)
			{
				if (o.has<jsonxx::Array>("projects"))
				{
					jsonxx::Array projectsArray = o.get<jsonxx::Array>("projects");

					for (size_t i = 0; i < projectsArray.size(); i++)
					{
						jsonxx::Object projectEntry = projectsArray.get<jsonxx::Object>(i);

						ProjectEntry entry;
						entry.name = projectEntry.get<jsonxx::String>("name");
						entry.path = projectEntry.get<jsonxx::String>("path");

						_RecentProjectMap.insert({ entry.name, entry });
					}
				}
			}
		}
	}

	bool RecentProjects::AddProject(const std::string& name, const std::string& path)
	{
		if (_RecentProjectMap.find(name) == _RecentProjectMap.end())
		{
			ProjectEntry entry;
			entry.name = name;
			entry.path = path;

			_RecentProjectMap.insert({ entry.name, entry });

			return true;
		}

		return false;
	}

	bool RecentProjects::RemoveProject(const std::string& path)
	{
		// to be implemented
		return false;
	}

	bool RecentProjects::Save()
	{
		jsonxx::Object o;

		jsonxx::Array projectsArray;

		for (auto& projectEntry : _RecentProjectMap)
		{
			jsonxx::Object projectEntryObject;
			projectEntryObject << "name" << projectEntry.second.name; 
			projectEntryObject << "path" << projectEntry.second.path;

			projectsArray.append(projectEntryObject);
		}

		o << "projects" << projectsArray;

		std::ofstream out("recentProject.json");
		out << o.json();
		out.close();

		return true;
	}
}

