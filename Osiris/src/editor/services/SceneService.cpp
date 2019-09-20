#pragma once

#include "osrpch.h"
#include "SceneService.h"

#include "editor/services/ServiceManager.h"
#include "editor/loaders/SceneLoader.h"
#include "layers/Renderer2DLayer.h"

namespace Osiris::Editor
{
	void SceneService::OnCreate() 
	{
		_loadedScene = std::make_shared<Scene>();
		_loadedScene->BuildDefaults();

		ServiceManager::Get<EventService>(ServiceManager::Events).Subscribe(Events::EventType::ProjectLoaded, [this](Events::EventArgs& args) {
			Events::ProjectLoadedArgs& a = (Events::ProjectLoadedArgs&)args;

			LoadScene(a.projectPath + "/Assets/" + a.initialScene + ".scene.json");
		});
	}

	void SceneService::OnDestroy() {}
	
	bool SceneService::CreateNewScene()
	{
		_loadedScene.reset();

		return true;
	}

	bool SceneService::LoadScene(std::string& path)
	{
		SceneLoader::Result result = SceneLoader::Load(path, *_loadedScene, FileContent::Json);

		_loadedScenePath = path;

		if (result != SceneLoader::Result::Success)
		{	
			switch (result)
			{
			case SceneLoader::Result::FileNotFound:
				break;
			case SceneLoader::Result::FileInUse:
				break;
			case SceneLoader::Result::FileMalformed:
				break;
			}

			return false;
		}
		else
		{
			/* Retrieve the current 2D rendering layer */
			Layers::Renderer2DLayer* renderer2DLayer = (Layers::Renderer2DLayer*)Application::Get().GetLayerStack()->FindLayer("2D Rendering Layer");

			/* Clear out all the sprite layers */
			renderer2DLayer->RemoveAllSpriteLayers();

			/* trigger a scene swap on the core system */

			/* fire scene loaded event on the editor system */
			ServiceManager::Get<EventService>(ServiceManager::Events).Publish(Editor::Events::SceneOpened, Events::SceneOpenedArgs());
			return true;
		}
	}

	bool SceneService::SaveScene()
	{
		SceneLoader::Result result = SceneLoader::Save(_loadedScenePath, *_loadedScene, FileContent::Json);

		if (result != SceneLoader::Result::Success)
		{
			switch (result)
			{
			case SceneLoader::Result::DirectoryNotFound:
				break;
			case SceneLoader::Result::FileAlreadyExists:
				break;
			case SceneLoader::Result::InsufficientSpace:
				break;
			}

			return false;
		}
		else
		{
			return true;
		}
	}

	bool SceneService::SaveScene(std::string& path)
	{
		SceneLoader::Result result = SceneLoader::Save(path, *_loadedScene, FileContent::Json);

		if (result != SceneLoader::Result::Success)
		{
			switch (result)
			{
			case SceneLoader::Result::DirectoryNotFound:
				break;
			case SceneLoader::Result::FileAlreadyExists:
				break;
			case SceneLoader::Result::InsufficientSpace:
				break;
			}

			return false;
		}
		else
		{
			return true;
		}
	}
}
