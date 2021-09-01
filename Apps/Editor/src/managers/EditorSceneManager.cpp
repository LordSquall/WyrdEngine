#pragma once

#include <core/Log.h>

#include "services/ServiceManager.h"
#include "EditorSceneManager.h"
#include "core/Application.h"

namespace Wyrd::Editor
{
	bool EditorSceneManager::ChangeScene(std::string& sceneName)
	{
		/* retrieve the services */
		auto workspaceService = ServiceManager::Get<WorkspaceService>();
		auto resourceService = ServiceManager::Get<ResourceService>();

		/* find the scene name */
		auto scene = resourceService->GetSceneResourceByName(sceneName);

		if (scene != nullptr)
		{
			if (workspaceService->LoadScene(scene->GetPath()))
			{
				WYRD_CORE_INFO("Scene Loaded: {0}", sceneName);
				return true;
			}
			else
			{
				WYRD_CORE_ERROR("Failed to load scene: {0}", sceneName);
				return false;
			}
		}
		else
		{
			WYRD_CORE_ERROR("Unable to find scene: {0}", sceneName);
			return false;
		}
	}


	bool EditorSceneManager::ResetInitialScene()
	{
		auto workspaceService = ServiceManager::Get<WorkspaceService>();

		if (workspaceService->LoadScene(_InitialScenePath))
		{
			WYRD_CORE_INFO("Scene Loaded: {0}", _InitialScenePath);
			return true;
		}
		else
		{
			WYRD_CORE_ERROR("Failed to load scene: {0}", _InitialScenePath);
			return false;
		}
	}
}