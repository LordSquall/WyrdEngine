#pragma once

#include "osrpch.h"

#include "SceneRes.h"


namespace Osiris::Editor
{
	SceneRes::SceneRes(const std::string& path) : Resource(Utils::GetFilename(path, false), path)
	{
	}

	bool SceneRes::Load()
	{
		return true;
	}
}
