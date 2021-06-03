#pragma once

#include "ControlLayer.h"
#include "import/ImportManager.h"
#include "managers/SceneManager.h"

#include <core/Log.h>
#include <core/Application.h>
#include <core/scene/Scene.h>

#include <fstream>

bool ControlLayer::OnAttach()
{
	baseDirectory = "C:/Projects/GameProjects/DemoProject/.builds/";

	
	if (!LoadCoreFile())
	{
		WYRD_CORE_ERROR("Unable to load Core File");
		return false;
	}

	if (!LoadGameFile())
	{
		WYRD_CORE_ERROR("Unable to load Game File");
		return false;
	}

	if (!LoadCommonBundleFile())
	{
		WYRD_CORE_ERROR("Unable to load Common Bundle File");
		return false;
	}

	auto& sceneManager = SceneManager::getInstance();

	sceneManager.LoadScene(baseDirectory, sceneManager.SetInitialScene());

	auto scene = SceneManager::getInstance().GetScene();

	Application::Get().GetBehaviour().Start(std::shared_ptr<Wyrd::Scene>(scene));

	return true;
}

void ControlLayer::OnDetach() { }

void ControlLayer::OnUpdate(Timestep ts) 
{
	auto scene = SceneManager::getInstance().GetScene();


	if (scene != nullptr)
	{
		Application::Get().GetBehaviour().Update(ts);

		
	}
}

void ControlLayer::OnRender(Timestep ts, Renderer& renderer)
{ 
	auto scene = SceneManager::getInstance().GetScene();

}

bool ControlLayer::LoadCoreFile()
{
	ImportManager::ImportCore(baseDirectory);
	return true;
}

bool ControlLayer::LoadGameFile()
{
	ImportManager::ImportGame(baseDirectory);
	return true;
}

bool ControlLayer::LoadCommonBundleFile()
{
	ImportManager::ImportCommonBundle(baseDirectory);
	return true;
}