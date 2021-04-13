#pragma once

#include <osrpch.h>
#include <core/Log.h>
#include <core/Application.h>
#include <core/Resources.h>
#include <core/renderer/Shader.h>

/* export headers */
#include "CoreFormat.h"
#include "CoreFunctions.h"
#include "GameFormat.h"
#include "GameFunctions.h"
#include "BundleFormat.h"
#include "BundleFunctions.h"
#include "SceneFormat.h"
#include "SceneFunctions.h"

#include "ExportManager.h"
#include "services/ServiceManager.h"
#include "services/WorkspaceService.h"
#include "support/Utils.h"


namespace Osiris::Editor
{
	void ExportManager::Export()
	{
		auto workspaceService = ServiceManager::Get<WorkspaceService>(ServiceManager::Workspace);
		std::shared_ptr<Project> project = workspaceService->GetCurrentProject();
		std::shared_ptr<Scene> scene = workspaceService->GetLoadedScene();

		std::ofstream commonBundle;
		
		std::string bundlesDir = Utils::GetBuildsFolder() + "/bundles/";
		Utils::CreateFolder(bundlesDir);

		GenerateCoreFile();
		GenerateGameFile();
		GenerateCommonBundleFile();

		for (auto& exportableScene : project->GetExportSettings().exportableScenes)
		{
			GenerateSceneBundleFile(exportableScene.first, bundlesDir);
		}

		OSR_CORE_TRACE("Exported Game Files");
	}

	void ExportManager::GenerateCoreFile()
	{
		std::ofstream core;
		core.open(Utils::GetBuildsFolder() + "/Core.dat", std::ios::out | std::ios::binary);

		/* bundle configuration header */
		CoreFormat_Config config;
		config.version = "1.0.0";
		Write(core, config);

		core.close();
	}

	void ExportManager::GenerateGameFile()
	{
		auto workspaceService = ServiceManager::Get<WorkspaceService>(ServiceManager::Workspace);
		std::shared_ptr<Project> project = workspaceService->GetCurrentProject();

		std::ofstream game;
		game.open(Utils::GetBuildsFolder() + "/Game.dat", std::ios::out | std::ios::binary);

		/* Game Configuration */
		GameFormat_Config config;
		config.version = "1.0.0";
		config.name = project->name;
		Write(game, config);

		/* Display Configuration */
		GameFormat_Display display;
		display.width = (uint32_t)project->GetExportSettings().width;
		display.height = (uint32_t)project->GetExportSettings().height;
		Write(game, display);

		/* Scene Listings */
		GameFormat_SceneListings sceneListings;
		for (auto& exportableScene : project->GetExportSettings().exportableScenes)
		{
			sceneListings.sceneUIDs.push_back(exportableScene.first);
		}
		sceneListings.sceneUIDs_cnt = project->GetExportSettings().exportableScenes.size();
		sceneListings.initialScene = project->GetExportSettings().initialScene;
		Write(game, sceneListings);

		game.close();
	}

	void ExportManager::GenerateCommonBundleFile()
	{
		std::ofstream commonBundle;
		commonBundle.open(Utils::GetBuildsFolder() + "/common.bundle", std::ios::out | std::ios::binary);

		/* bundle configuration header */
		BundleFormat_Config config;
		config.version = "1.0.0";
		Write(commonBundle, config);

		commonBundle.close();
	}

	void ExportManager::GenerateSceneBundleFile(const Osiris::UID& sceneUID, const std::string& dir)
	{
		std::ofstream sceneBundle;
		sceneBundle.open(dir + sceneUID.str() + ".bundle", std::ios::out | std::ios::binary);

		/* scene configuration header */
		SceneFormat_Config config;
		config.version = "1.0.0";
		Write(sceneBundle, config);

		sceneBundle.close();
	}
}