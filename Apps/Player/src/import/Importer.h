#pragma once

#include <osrpch.h>

#include "import/core/CoreFormat.h"
#include "import/game/GameFormat.h"
#include "import/bundle/BundleFormat.h"
#include "import/scene/SceneFormat.h"

class Importer
{
public:
	/* Core format */
	static void Read(std::ifstream& stream, CoreFormat_Config& config);

	/* Game format */
	static void Read(std::ifstream& stream, GameFormat_Config& config);
	static void Read(std::ifstream& stream, GameFormat_Display& display);
	static void Read(std::ifstream& stream, GameFormat_SceneListings& sceneListings);

	/* Bundle format */
	static void Read(std::ifstream& stream, BundleFormat_Config& config);

	/* Scene format */
	static void Read(std::ifstream& stream, SceneFormat_Config& config);

	/* Helper functions */
	static uint32_t UIntHelper(std::ifstream& stream);
	static std::string StringHelper(std::ifstream& stream);
};