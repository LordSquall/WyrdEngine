#pragma once

#include <core/Log.h>

#include "SceneManager.h"
#include "core/Application.h"
#include "import/ImportManager.h"

bool SceneManager::ChangeScene(std::string& sceneName)
{
	std::string baseDirectory = "C:/Projects/GameProjects/RPG/.builds/";

	if (_SceneMap.find(sceneName) != _SceneMap.end())
	{
		_Scene.release();

		LoadScene(baseDirectory, _SceneMap[sceneName]);

		Application::Get().GetBehaviour().Restart(std::shared_ptr<Wyrd::Scene>(SceneManager::getInstance().GetScene()));
	}

	return false;
}


bool SceneManager::AddScene(Wyrd::UID& uid, std::string& name)
{
	_SceneMap.emplace(name, uid);

	return true;
}

bool SceneManager::LoadInitialScene(const std::string& root)
{
	WYRD_TRACE("Loading Initial Scene: {0}", _InitialScene);

	_Scene = std::move(ImportManager::ImportScene(root, _InitialScene));

	return true;
}

bool SceneManager::LoadScene(const std::string& root, const Wyrd::UID& uid)
{
	WYRD_TRACE("Loading Scene: {0}", uid);

	_Scene = std::move(ImportManager::ImportScene(root, uid));

	return true;
}