#pragma once

#include "osrpch.h"

#include "SceneRes.h"

#include <nlohmann/json.hpp>

namespace Osiris::Editor
{
	SceneRes::SceneRes(const std::string& path) : _path(path)
	{
		/* load the scene json to extract the scene name */
		std::ifstream i(path);
		nlohmann::json j;
		i >> j;

		_name = Utils::GetFilename(path, false);
	}
}
