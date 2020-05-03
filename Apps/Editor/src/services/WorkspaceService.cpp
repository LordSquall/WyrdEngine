#pragma once

#include "osrpch.h"
#include "ServiceManager.h"
#include "WorkspaceService.h"
#include "loaders/ProjectLoader.h"
#include "loaders/SceneLoader.h"

#include <nlohmann/json.hpp>

using namespace nlohmann;

namespace Osiris::Editor
{
	void Osiris::Editor::WorkspaceService::OnCreate()
	{
		/* Initialise Defaults */
		IsProjectLoaded(false);

		/* Subscribe to Project lifecycle events */
		ServiceManager::Get<EventService>(ServiceManager::Events)->Subscribe(Events::EventType::CreateNewProject, [this](Events::EventArgs& args) {
			Events::CreateNewProjectArgs& a = (Events::CreateNewProjectArgs&)args;
			CreateNewProject(a.location, a.name);
			});

		ServiceManager::Get<EventService>(ServiceManager::Events)->Subscribe(Events::EventType::OpenProject, [this](Events::EventArgs& args) {});

		ServiceManager::Get<EventService>(ServiceManager::Events)->Subscribe(Events::EventType::CloseProject, [this](Events::EventArgs& args) {});
	}

	void Osiris::Editor::WorkspaceService::OnDestroy()
	{

	}

	void Osiris::Editor::WorkspaceService::CreateNewProject(std::string location, std::string name)
	{
		/* Build project file name */
		_LoadedProjectPath = location + "/" + name + "/" + name + ".oproj";

		/* Create a new project shared pointer */
		_LoadedProject = std::make_shared<Project>();

		/* Create the folder for the project */
		Utils::CreateFolder(location + "/" + name);

		/* Save the project object to disk */
		ProjectLoader::Result result = ProjectLoader::Save(_LoadedProjectPath, *_LoadedProject, Osiris::FileContent::Json);

		if (result == ProjectLoader::Result::Success)
		{
			/* Mark project as loaded */
			IsProjectLoaded(true);

			/* Set the utilities to the base root folder */
			Utils::SetRootProjectFolder(Utils::GetPath(_LoadedProjectPath.c_str()));

			/* Create default folders and files */
			Utils::CreateProjectFileStructure(Utils::GetPath(_LoadedProjectPath));

			/* Send a Project Loaded Event */
			ServiceManager::Get<EventService>(ServiceManager::Events)->Publish(Events::EventType::ProjectLoaded,
				Events::ProjectLoadedArgs(_LoadedProject->initialScene, _LoadedProject, Utils::GetPath(_LoadedProjectPath)));
		}
	}

	bool Osiris::Editor::WorkspaceService::SaveProject()
	{
		ProjectLoader::Result result = ProjectLoader::Save(_LoadedProjectPath, *_LoadedProject, FileContent::Json);

		if (result != ProjectLoader::Result::Success)
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

	void Osiris::Editor::WorkspaceService::LoadProject(std::string projectfile)
	{
		/* Store the loaded project path */
		_LoadedProjectPath = projectfile;

		/* Create a new project shared pointer */
		_LoadedProject = std::make_shared<Project>();

		/* Load the project object */
		ProjectLoader::Result result = ProjectLoader::Load(projectfile.c_str(), *_LoadedProject, Osiris::FileContent::Json);

		if (result == ProjectLoader::Result::Success)
		{
			/* Mark project as loaded */
			IsProjectLoaded(true);

			/* Set the utilities to the base root folder */
			Utils::SetRootProjectFolder(Utils::GetPath(projectfile.c_str()));

			/* Send a Project Loaded Event */
			ServiceManager::Get<EventService>(ServiceManager::Events)->Publish(Events::EventType::ProjectLoaded,
				Events::ProjectLoadedArgs(_LoadedProject->initialScene, _LoadedProject, Utils::GetPath(projectfile)));

			if (Utils::FileExists(_LoadedProject->initialScene) == true)
			{
				LoadScene(_LoadedProject->initialScene);
			}
			else
			{
				OSR_WARN("Unable to find scene {0}. Creating new Scene...", _LoadedProject->initialScene);
				CreateNewScene();
			}
		}
	}

	bool WorkspaceService::CreateNewScene()
	{
		/* Create a new scene shared pointer */
		_LoadedScene = std::make_shared<Scene>();

		/* Mark scene loaded */
		IsSceneLoaded(true);

		/* fire scene loaded event on the editor system */
		ServiceManager::Get<EventService>(ServiceManager::Events)->Publish(Editor::Events::EventType::SceneOpened, Events::SceneOpenedArgs(_LoadedScene));

		return true;
	}

	bool WorkspaceService::LoadScene(const std::string& path)
	{
		_LoadedScene = std::make_shared<Scene>();

		/* Load the scene object */
		SceneLoader::Result result = SceneLoader::Load(path, *_LoadedScene, Osiris::FileContent::Json);

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
			_LoadedScenePath = path;

			/* fire scene loaded event on the editor system */
			ServiceManager::Get<EventService>(ServiceManager::Events)->Publish(Editor::Events::EventType::SceneOpened, Events::SceneOpenedArgs(_LoadedScene));

			return true;
		}
	}

	bool WorkspaceService::SaveScene()
	{
		SceneLoader::Result result = SceneLoader::Save(_LoadedScenePath, *_LoadedScene, FileContent::Json);

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
			_LoadedProject->initialScene = _LoadedScenePath;
			SaveProject();

			return true;
		}
	}

	bool WorkspaceService::SaveSceneAs(const std::string& path)
	{
		SceneLoader::Result result = SceneLoader::Save(path, *_LoadedScene, FileContent::Json);

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
			_LoadedProject->initialScene = _LoadedScenePath;
			SaveProject();

			return true;
		}
	}
}
