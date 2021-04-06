#pragma once

#include <osrpch.h>

struct GameFormat_Config
{
	std::string version;
	std::string name;
};

struct GameFormat_Display
{
	uint32_t width;
	uint32_t height;
};

struct GameFormat_SceneListings
{
	std::vector<std::string> sceneUIDs;
	std::string initialScene;
};