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
		/* Initialise default values */
		IsSceneLoaded(false);

		/* Subscribe to project lifecycle events */
		ServiceManager::Get<EventService>(ServiceManager::Events)->Subscribe(Events::EventType::ProjectLoaded, [this](Events::EventArgs& args) {
			Events::ProjectLoadedArgs& a = (Events::ProjectLoadedArgs&)args;

			std::string fileName = a.projectPath + "/Assets/" + a.initialScene + ".scene";
			
			Utils::FileExists(fileName) ? LoadScene(fileName) : CreateNewScene();
		});
	}

	void SceneService::OnDestroy() {}
	
	bool SceneService::CreateNewScene()
	{
		/* Create a new scene shared pointer */
		_loadedScene = std::make_shared<Scene>();

		/* Mark scene loaded */
		IsSceneLoaded(true);

		/* fire scene loaded event on the editor system */
		ServiceManager::Get<EventService>(ServiceManager::Events)->Publish(Editor::Events::SceneOpened, Events::SceneOpenedArgs(_loadedScene));

		return true;
	}

	bool SceneService::LoadScene(std::string& path)
	{
		_loadedScene = std::make_shared<Scene>();

		/* Load the scene object */
		SceneLoader::Result result = SceneLoader::Load(path, *_loadedScene, Osiris::FileContent::Json);

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
			/* Mark scene loaded */
			IsSceneLoaded(true);

			/* Update the loaded scene path */
			_loadedScenePath = path;

			/* fire scene loaded event on the editor system */
			ServiceManager::Get<EventService>(ServiceManager::Events)->Publish(Editor::Events::SceneOpened, Events::SceneOpenedArgs(_loadedScene));
			
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
