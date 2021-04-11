#pragma once

#include "ControlLayer.h"
#include "import/ImportManager.h"

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
	ImportManager::ImportCore();
	return true;
}

bool ControlLayer::LoadGameFile()
{
	ImportManager::ImportGame();
	return true;
}

bool ControlLayer::LoadCommonBundleFile()
{
	ImportManager::ImportCommonBundle();
	return true;
}