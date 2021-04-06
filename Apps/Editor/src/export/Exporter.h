#pragma once

#include <osrpch.h>

#include "Apps/Player/src/import/core/CoreFormat.h"
#include "Apps/Player/src/import/game/GameFormat.h"
#include "Apps/Player/src/import/bundle/BundleFormat.h"
#include "Apps/Player/src/import/scene/SceneFormat.h"

class Exporter
{
public:
	/* Core format */
	static void Write(std::ofstream& stream, CoreFormat_Config& config);

	/* Game format */
	static void Write(std::ofstream& stream, GameFormat_Config& config);
	static void Write(std::ofstream& stream, GameFormat_Display& display);
	static void Write(std::ofstream& stream, GameFormat_SceneListings& sceneListings);

	/* Bundle format */
	static void Write(std::ofstream& stream, BundleFormat_Config& config);

	/* Scene format */
	static void Write(std::ofstream& stream, SceneFormat_Config& config);

	/* Helper functions */
	static void UIntHelper(std::ofstream& stream, uint32_t val);
	static void StringHelper(std::ofstream& stream, std::string& str);
};