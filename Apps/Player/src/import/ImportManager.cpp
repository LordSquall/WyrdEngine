#pragma once

#include <osrpch.h>
#include <core/Log.h>
#include <core/Application.h>

#include "import/ImportManager.h"

#include "CoreFormat.h"
#include "CoreFunctions.h"
#include "GameFormat.h"
#include "GameFunctions.h"
#include "BundleFormat.h"
#include "BundleFunctions.h"

void ImportManager::ImportCore()
{
	std::string baseDirectory = "C:/Projects/GameProjects/DemoProject/.builds/";

	/* open the meta data file */
	std::ifstream core;
	core.open(baseDirectory + "Core.dat", std::ios::in | std::ios::binary);
	if (core.good() == false)
	{
		OSR_CORE_ERROR("Unable to find core.dat file");
		return;
	}

	/* core file configuration */
	CoreFormat_Config config;
	Read(core, config);

	OSR_TRACE("Core Config Version: {0}", config.version);

	core.close();
}

void ImportManager::ImportGame()
{
	std::string baseDirectory = "C:/Projects/GameProjects/DemoProject/.builds/";

	/* open the file */
	std::ifstream game;
	game.open(baseDirectory + "Game.dat", std::ios::in | std::ios::binary);
	if (game.good() == false)
	{
		OSR_CORE_ERROR("Unable to find game.dat file");
		return;
	}

	/* game file configuration */
	GameFormat_Config config;
	Read(game, config);

	OSR_TRACE("Game Config Version: {0}", config.version);
	
	Osiris::Application::Get().GetWindow().SetTitle(config.name + " - " + config.version);

	///* display configuration */
	GameFormat_Display display;
	Read(game, display);

	Osiris::Application::Get().GetWindow().SetSize(display.width, display.height);

	/* scene listings */
	GameFormat_SceneListings sceneListings;
	Read(game, sceneListings);

	for (auto& listing : sceneListings.sceneUIDs) OSR_TRACE("Scene Listing: {0}", listing);

	OSR_TRACE("Initial Scene: {0}", sceneListings.initialScene);

	/* close the file */
	game.close();
}

void ImportManager::ImportCommonBundle()
{
	std::string baseDirectory = "C:/Projects/GameProjects/DemoProject/.builds/";

	/* open the meta data file */
	std::ifstream common;
	common.open(baseDirectory + "common.bundle", std::ios::in | std::ios::binary);
	if (common.good() == false)
	{
		OSR_CORE_ERROR("Unable to find common.bundle file");
		return;
	}

	/* common file configuration */
	BundleFormat_Config config;
	Read(common, config);

	OSR_TRACE("Common Config Version: {0}", config.version);

	common.close();
}

void ImportManager::ImportScene()
{

}