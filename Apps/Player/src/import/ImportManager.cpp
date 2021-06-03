#pragma once

#include <wyrdpch.h>
#include <core/Log.h>
#include <core/Application.h>
#include <core/renderer/Shader.h>
#include <core/renderer/Texture.h>
#include <core/behaviour/ScriptedClass.h>

#include "import/ImportManager.h"
#include "managers/SceneManager.h"


void ImportManager::ImportCore(const std::string& root)
{
	/* open the meta data file */
	std::ifstream core;
	core.open(root + "Core.dat", std::ios::in | std::ios::binary);
	if (core.good() == false)
	{
		WYRD_CORE_ERROR("Unable to find core.dat file");
		return;
	}

	core.close();
}

void ImportManager::ImportGame(const std::string& root)
{
	/* open the file */
	std::ifstream game;
	game.open(root + "Game.dat", std::ios::in | std::ios::binary);
	if (game.good() == false)
	{
		WYRD_CORE_ERROR("Unable to find game.dat file");
		return;
	}

	/* close the file */
	game.close();
}

void ImportManager::ImportCommonBundle(const std::string& root)
{
	/* open the meta data file */
	std::ifstream common;
	common.open(root + "common.bundle", std::ios::in | std::ios::binary);
	if (common.good() == false)
	{
		WYRD_CORE_ERROR("Unable to find common.bundle file");
		return;
	}

	common.close();
}

std::unique_ptr<Wyrd::Scene> ImportManager::ImportScene(const std::string& root, const Wyrd::UID& sceneUID)
{
	/* open the meta data file */
	std::ifstream scene;
	scene.open(root + "bundles/" + sceneUID.str().c_str() + ".bundle", std::ios::in | std::ios::binary);
	if (scene.good() == false)
	{
		WYRD_CORE_ERROR("Unable to find bundles/{0}.bundle file", sceneUID);
		return nullptr;
	}

	return nullptr;
}