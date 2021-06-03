#pragma once

#include <wyrdpch.h>
#include <core/Log.h>
#include <core/Application.h>
#include <core/Resources.h>
#include <core/behaviour/Behaviour.h>
#include <core/renderer/Shader.h>

/* export headers */
#include "loaders/SceneLoader.h"

#include "ExportManager.h"
#include "services/ServiceManager.h"
#include "services/WorkspaceService.h"
#include "support/Utils.h"


namespace Wyrd::Editor
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

		WYRD_CORE_TRACE("Exported Game Files");
	}

	void ExportManager::GenerateCoreFile()
	{
		std::ofstream core;
		core.open(Utils::GetBuildsFolder() + "/Core.dat", std::ios::out | std::ios::binary);

		core.close();
	}

	void ExportManager::GenerateGameFile()
	{
		auto workspaceService = ServiceManager::Get<WorkspaceService>(ServiceManager::Workspace);
		std::shared_ptr<Project> project = workspaceService->GetCurrentProject();

		std::ofstream game;
		game.open(Utils::GetBuildsFolder() + "/Game.dat", std::ios::out | std::ios::binary);

		game.close();
	}

	void ExportManager::GenerateCommonBundleFile()
	{
		auto resourceService = ServiceManager::Get<ResourceService>(ServiceManager::Resources);
		auto workspaceService = ServiceManager::Get<WorkspaceService>(ServiceManager::Workspace);

		std::ofstream commonBundle;
		commonBundle.open(Utils::GetBuildsFolder() + "/common.bundle", std::ios::out | std::ios::binary);

		commonBundle.close();
	}

	void ExportManager::GenerateSceneBundleFile(const Wyrd::UID& sceneUID, const std::string& dir)
	{
		std::ofstream sceneBundle;
		sceneBundle.open(dir + sceneUID.str() + ".bundle", std::ios::out | std::ios::binary);

		/* Load the matching scene */
		auto resourceService = ServiceManager::Get<ResourceService>(ServiceManager::Resources);
		auto sceneRes = resourceService->GetResourceByID<SceneRes>(sceneUID);

		Scene scene;
		SceneLoader::Load(sceneRes->GetPath(), scene);

		sceneBundle.close();
	}
}