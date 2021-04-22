#pragma once

#include "ControlLayer.h"
#include "import/ImportManager.h"
#include "managers/SceneManager.h"

#include <core/Log.h>
#include <core/Application.h>
#include <core/scene/Scene.h>
#include <core/scene/SceneLayer2D.h>
#include <core/scene/components/CameraComponent.h>
#include <core/scene/components/Transform2DComponent.h>

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

	auto& sceneManager = SceneManager::getInstance();

	sceneManager.LoadScene(baseDirectory, sceneManager.SetInitialScene());

	auto scene = SceneManager::getInstance().GetScene();

	Application::Get().GetBehaviour().Start(std::shared_ptr<Osiris::Scene>(scene));

	return true;
}

void ControlLayer::OnDetach() { }

void ControlLayer::OnUpdate(Timestep ts) 
{
	auto scene = SceneManager::getInstance().GetScene();


	if (scene != nullptr)
	{
		Application::Get().GetBehaviour().Update(ts);

		for (auto& sl : scene->GetLayers())
		{
			for (auto& go : sl->GetGameObjects())
			{
				go->transform->Recalculate();
				for (auto& component : go->components)
				{
					component->Recalculate();
				}
			}
		}
	}
}

void ControlLayer::OnRender(Timestep ts, Renderer& renderer)
{ 
	auto scene = SceneManager::getInstance().GetScene();

	auto camera = scene->GetPrimaryCamera();

	Transform2DComponent* transform2D = dynamic_cast<Transform2DComponent*>(camera->Owner->transform.get());

	camera->GetCamera().SetPosition(vec3(transform2D->position, 0.0f));
	camera->GetCamera().RecalulateViewProjection();

	if (scene)
	{
		renderer.Clear(scene->bgcolor.r, scene->bgcolor.g, scene->bgcolor.b);

		for (auto& l : scene->GetLayers())
		{
			auto layer2D = dynamic_cast<SceneLayer2D*>(l.get());
			layer2D->GetSpriteBatch()->Render(renderer, camera->GetCamera().GetViewProjectionMatrix());
		}
	}
	else
	{
		renderer.Clear(1.0, 0.0, 0.0);
	}
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