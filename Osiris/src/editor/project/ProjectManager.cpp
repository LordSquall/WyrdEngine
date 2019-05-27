#pragma once

#include "osrpch.h"

#include "ProjectManager.h"

#include <nlohmann/json.hpp>

using namespace nlohmann;

namespace Osiris::Editor
{
	ProjectManager* ProjectManager::s_Instance = nullptr;

	ProjectManager& ProjectManager::Get()
	{
		if (s_Instance == nullptr)
		{
			s_Instance = new ProjectManager();
		}

		return *s_Instance;
	}

	bool ProjectManager::Load(std::string& filepath)
	{
		nlohmann::json;
		std::ifstream i(filepath);
		json j;
		i >> j;

		settings.name = j["name"].get<std::string>();

		settings.rootDir = filepath.substr(0, filepath.find_last_of('/')) + '/';

		for (auto& el : j["resource-dirs"].items()) {
			for (const auto & entry : std::filesystem::directory_iterator(settings.rootDir + el.value().get<std::string>()))
			{
				std::shared_ptr<Osiris::Texture> texture = Osiris::Texture::CreateFromFile(
					entry.path().string().c_str()
				);

				if (texture)
					Application::Get().GetResources().AddTexture(texture);

				std::cout << entry.path() << std::endl;
			}
		}
		OSR_CORE_TRACE("Project Name: {0}", settings.name);

		return true;
	}
}