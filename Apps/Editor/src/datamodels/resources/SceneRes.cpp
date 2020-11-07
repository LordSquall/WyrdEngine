#pragma once

#include "osrpch.h"

#include "SceneRes.h"


namespace Osiris::Editor
{
	SceneRes::SceneRes(const std::string& path) : _path(path)
	{
		_name = Utils::GetFilename(path, false);
	}
}
