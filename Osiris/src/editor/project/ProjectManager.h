#pragma once

#include "core/export.h"
#include "core/Layer.h"

#include "core/Application.h"

#include "ProjectSettings.h"

namespace Osiris::Editor
{
	class ProjectManager
	{
	public:

		static ProjectManager& Get();

		bool Load(std::string& filepath);

		ProjectSettings settings;

	private:
		static ProjectManager* s_Instance;

		std::vector<std::string> m_ResourceLocations;
	};
}