#pragma once
#include "osrpch.h"

#include "ProjectLoader.h"

#include <nlohmann/json.hpp>

using namespace nlohmann;

namespace Osiris::Editor
{
	/* Json operation overloader functions */
	void to_json(json& jProject, const Project& project);
	void from_json(const json& jProject, Project& project);

	ProjectLoader::Result ProjectLoader::Load(std::string path, Project& project, FileContent content)
	{
		json j;
		std::ifstream i(path);

		if (i.is_open())
		{
			i >> j;
			project = j.get<Project>();
			return Success;
		}
		return FileNotFound;
	}

	ProjectLoader::Result ProjectLoader::Save(std::string path, Project& project, FileContent content)
	{
		json j = project;

		std::ofstream o(path);

		o << std::setw(4) << j << std::endl;

		o.close();

		return Success;
	}

	void to_json(json& jProject, const Project& project) {
		jProject = json::object();

		jProject["name"] = project.name;
		jProject["initialScene"] = project.initialScene;
	}

	void from_json(const json& jProject, Project& project) {
		jProject.at("name").get_to(project.name);
		jProject.at("initialScene").get_to(project.initialScene);
	}
}