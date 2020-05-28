#pragma once

#include "osrpch.h"

#include "ScriptRes.h"

#include "services/ServiceManager.h"

#include <nlohmann/json.hpp>

namespace Osiris::Editor
{
	ScriptRes::ScriptRes(const std::string& path) : _path(path)
	{
		/* store the resolved name for the resource. This is important at the moment because all the resource management assumes unique names */
		_name = Utils::GetFilename(path, false);

		/* get the source code from the lua file */
		std::ifstream t(path);
		std::string source = std::string((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());
		
		/* create a new scriped object template from the source code */
		ScriptedObjectTemplate = ServiceManager::Get<SimulationService>(ServiceManager::Simulation)->CreateScriptableObjectTemplate(source);
	}
}
