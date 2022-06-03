#include "wyrdpch.h"

#include "SceneRes.h"


namespace Wyrd::Editor
{
	SceneRes::SceneRes(const std::filesystem::path& filepath) : Resource(filepath.stem().string(), filepath)
	{
	}

	bool SceneRes::Load()
	{
		return true;
	}
}
