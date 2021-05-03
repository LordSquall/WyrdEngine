#pragma once

#include "wyrdpch.h"

#include "SceneRes.h"


namespace Wyrd::Editor
{
	SceneRes::SceneRes(const std::string& path) : Resource(Utils::GetFilename(path, false), path)
	{
	}

	bool SceneRes::Load()
	{
		return true;
	}
}
