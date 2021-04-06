#pragma once

#include "ControlLayer.h"
#include "import/Importer.h"

#include <core/Log.h>
#include <core/Application.h>

#include <fstream>

bool ControlLayer::OnAttach()
{
	baseDirectory = "C:/Projects/GameProjects/DemoProject/.builds/";

	if (!LoadCoreFile())
	{
		OSR_CORE_ERROR("Unable to load Core File");
		return false;
	}

	if (!LoadGameFile())
	{
		OSR_CORE_ERROR("Unable to load Game File");
		return false;
	}

	if (!LoadCommonBundleFile())
	{
		OSR_CORE_ERROR("Unable to load Common Bundle File");
		return false;
	}

	return true;
}

void ControlLayer::OnDetach() { }

void ControlLayer::OnUpdate(Timestep ts) { }

void ControlLayer::OnRender(Timestep ts, Renderer& renderer)
{ 
	renderer.Clear(1.0f, 0.0f, 0.0f);
}

bool ControlLayer::LoadCoreFile()
{
	/* open the meta data file */
	std::ifstream core;
	core.open(baseDirectory + "Core.dat", std::ios::in | std::ios::binary);
	if (core.good() == false)
	{
		OSR_CORE_ERROR("Unable to find core.dat file");
		return NULL;
	}

	/* core file configuration */
	CoreFormat_Config config;
	Importer::Read(core, config);

	OSR_TRACE("Core Config Version: {0}", config.version);

	core.close();

	return true;
}

bool ControlLayer::LoadGameFile()
{
	/* open the file */
	std::ifstream game;
	game.open(baseDirectory + "Game.dat", std::ios::in | std::ios::binary);
	if (game.good() == false)
	{
		OSR_CORE_ERROR("Unable to find game.dat file");
		return NULL;
	}

	/* game file configuration */
	GameFormat_Config config;
	Importer::Read(game, config);

	OSR_TRACE("Game Config Version: {0}", config.version);
	
	Application::Get().GetWindow().SetTitle(config.name + " - " + config.version);

	/* display configuration */
	GameFormat_Display display;
	Importer::Read(game, display);
	
	Application::Get().GetWindow().SetSize(display.width, display.height);

	/* scene listings */
	GameFormat_SceneListings sceneListings;
	Importer::Read(game, sceneListings);

	for (auto& listing : sceneListings.sceneUIDs)
		OSR_TRACE("Scene Listing: {0}", listing);
	OSR_TRACE("Initial Scene: {0}", sceneListings.initialScene);

	/* close the file */
	game.close();

	return true;
}

bool ControlLayer::LoadCommonBundleFile()
{
	/* open the file */
	std::ifstream bundle;
	bundle.open(baseDirectory + "common.bundle", std::ios::in | std::ios::binary);
	if (bundle.good() == false)
	{
		OSR_CORE_ERROR("Unable to find game.dat file");
		return NULL;
	}

	/* game file configuration */
	BundleFormat_Config config;
	Importer::Read(bundle, config);

	OSR_TRACE("Bundle Config Version: {0}", config.version);

	return true;
}