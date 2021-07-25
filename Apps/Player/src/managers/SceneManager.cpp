#pragma once

#include <core/Log.h>

#include "SceneManager.h"
#include "import/ImportManager.h"

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