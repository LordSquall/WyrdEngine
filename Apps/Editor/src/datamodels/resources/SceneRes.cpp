#include "wyrdpch.h"

#include "SceneRes.h"


namespace Wyrd::Editor
{
	SceneRes::SceneRes(const std::filesystem::path& filepath, const UID& uid) : Resource(filepath.stem().string(), filepath, uid)
	{
	}

	int SceneRes::Load()
	{
		return 0;
	}
}
