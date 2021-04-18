#pragma once

#include <core/Log.h>

#include "SceneManager.h"
#include "import/ImportManager.h"

bool SceneManager::LoadScene(const std::string& root, const Osiris::UID& uid)
{
	OSR_TRACE("Loading Scene: {0}", uid);

	_Scene = std::move(ImportManager::ImportScene(root, uid));

	return true;
}