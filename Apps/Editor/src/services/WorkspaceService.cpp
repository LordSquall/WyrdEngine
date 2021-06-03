#pragma once

/* core wyrd includes */
#include <wyrdpch.h>
#include <core/Log.h>

/* local includes */
#include "WorkspaceService.h"
#include "ServiceManager.h"
#include "loaders/SceneLoader.h"
#include "loaders/ProjectLoader.h"

/* external includes */
#include <efsw/efsw.hpp>
#include <jsonxx.h>

namespace Wyrd::Editor
{
	class FSUpdateListener : public efsw::FileWatchListener
	{
	public:
		FSUpdateListener() {}
		~FSUpdateListener() {}

		void handleFileAction(efsw::WatchID watchID, const std::string& dir, const std::string& filename, efsw::Action action, std::string oldFilename = "") override
		{
			bool isDir = Utils::GetFileExtension(filename).empty();

			switch (action)
			{
			case efsw::Actions::Add:
				ServiceManager::Get<EventService>(ServiceManager::Events)->Publish(Events::EventType::AddFileEntry, std::make_unique<Events::AddFileEntryArgs>(dir, dir + "\\" + filename, isDir), true);
				break;
			case efsw::Actions::Delete:
				ServiceManager::Get<EventService>(ServiceManager::Events)->Publish(Events::EventType::DeleteFileEntry, std::make_unique<Events::DeleteFileEntryArgs>(dir, dir + "\\" + filename, isDir), true);
				break;
			case efsw::Actions::Modified:				
				ServiceManager::Get<EventService>(ServiceManager::Events)->Publish(Events::EventType::ModifiedFileEntry, std::make_unique<Events::ModifiedFileEntryArgs>(dir, dir + "\\" + filename, isDir), true);
				break;
			case efsw::Actions::Moved:
				ServiceManager::Get<EventService>(ServiceManager::Events)->Publish(Events::EventType::RenameFileEntry, std::make_unique<Events::RenameFileEntryArgs>(dir, dir + "\\" + filename, oldFilename, isDir), true);

				break;
			default:
				std::cout << "Should never happen!" << std::endl;
			}
		}
	};

	efsw::FileWatcher	fileSystemWatcher;
	efsw::WatchID		watchID;
	FSUpdateListener	updateListener;

	void Wyrd::Editor::WorkspaceService::OnCreate()
	{
		/* Initialise Defaults */
		IsProjectLoaded(false);

		/* Subscribe to Project lifecycle events */
		ServiceManager::Get<EventService>(ServiceManager::Events)->Subscribe(Events::EventType::CreateNewProject, [this](Events::EventArgs& args) {
			Events::CreateNewProjectArgs& a = (Events::CreateNewProjectArgs&)args;
			CreateNewProject(a.location, a.sceneName, a.name);
		});

		ServiceManager::Get<EventService>(ServiceManager::Events)->Subscribe(Events::EventType::OpenProject, [this](Events::EventArgs& args) {});

		ServiceManager::Get<EventService>(ServiceManager::Events)->Subscribe(Events::EventType::CloseProject, [this](Events::EventArgs& args) {});

		ServiceManager::Get<EventService>(ServiceManager::Events)->Subscribe(Events::EventType::SetSceneCamera, [this](Events::EventArgs& args) {
			Events::SetSceneCameraArgs& a = (Events::SetSceneCameraArgs&)args;
			//_LoadedScene->SetPrimaryCameraUID(a.cameraComponent->GetCameraUID());
			});
	}

	void Wyrd::Editor::WorkspaceService::OnDestroy()
	{
		/* TEMP if (_LoadedProject != nullptr) */ SaveProject();
		if (_LoadedScene != nullptr) SaveScene();
	}

	void Wyrd::Editor::WorkspaceService::CreateNewProject(std::string location, std::string sceneName, std::string name)
	{
		/* Build project file name */
		_LoadedProjectPath = location + "\\" + name + "\\" + name + ".oproj";

		/* Create a new project shared pointer */
		_LoadedProject = std::make_shared<Project>(name);

		/* Create the folder for the project */
		Utils::CreateFolder(location + "\\" + name);

		/* Save the project object to disk */
		ProjectLoader::Result result = ProjectLoader::Save(_LoadedProjectPath, *_LoadedProject);

		if (result == ProjectLoader::Result::Success)
		{
			/* Mark project as loaded */
			IsProjectLoaded(true);

			/* Update the settings to ensure this project is loaded by default next time */
			ServiceManager::Get<SettingsService>(ServiceManager::Settings)->SetSetting(_LoadedProjectPath, CONFIG_PROJECT, CONFIG_PROJECT__DEFAULT);
				
			/* Set the utilities to the base root folder */
			Utils::SetRootProjectFolder(Utils::GetPath(_LoadedProjectPath.c_str()));

			/* Create default folders and files */
			Utils::CreateProjectFileStructure(Utils::GetPath(_LoadedProjectPath));

			/* Create a new default scene */
			CreateNewScene(sceneName);

			/* Set the default scene path */
			_LoadedScenePath = Utils::GetAssetFolder() + "\\" + sceneName + ".scene";

			/* Save the new scene into a known location */
			SaveScene();

			/* Set the default scene within the project model */
			_LoadedProject->initialScene = Utils::GetAssetFolder() + "\\" + sceneName + ".scene";

			/* create VS workspace file model */
			jsonxx::Object root;
			jsonxx::Array folders;
			jsonxx::Object assetsFolder;
			assetsFolder << "path" << "assets";
			folders << assetsFolder;

			root << "folders" << folders;
			std::ofstream out(Utils::GetPath(_LoadedProjectPath.c_str()) + "/vs.code-workspace");
			out << root.json();
			out.close();

			/* Send a Project Loaded Event */
			ServiceManager::Get<EventService>(ServiceManager::Events)->Publish(Events::EventType::ProjectLoaded,
				std::make_unique<Events::ProjectLoadedArgs>(_LoadedProject->initialScene, _LoadedProject, Utils::GetPath(_LoadedProjectPath)));
		}
	}

	bool Wyrd::Editor::WorkspaceService::SaveProject()
	{
		ProjectLoader::Result result = ProjectLoader::Result::Success;
		
		/* If a scene is loaded we want it to be set as the initial scene in the project model to ensure it gets loaded on restart */
		if (_LoadedScene != nullptr) _LoadedProject->initialScene = _LoadedScenePath;
		
		if (_LoadedProject != nullptr)
		{
			result = ProjectLoader::Save(_LoadedProjectPath, *_LoadedProject);

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
		return false;
	}

	bool Wyrd::Editor::WorkspaceService::LoadProject(std::string projectfile)
	{
		bool result = false;

		/* Store the loaded project path */
		_LoadedProjectPath = projectfile;

		/* Create a new project shared pointer */
		_LoadedProject = std::make_shared<Project>();

		/* Load the project object */
		ProjectLoader::Result r = ProjectLoader::Load(projectfile.c_str(), *_LoadedProject);

		if (r == ProjectLoader::Result::Success)
		{
			/* Mark project as loaded */
			IsProjectLoaded(true);

			/* Update the settings to ensure this project is loaded by default next time */
			ServiceManager::Get<SettingsService>(ServiceManager::Settings)->SetSetting(_LoadedProjectPath, CONFIG_PROJECT, CONFIG_PROJECT__DEFAULT);

			/* Set the utilities to the base root folder */
			Utils::SetRootProjectFolder(Utils::GetPath(projectfile.c_str()));

			/* Start filesystem watcher */
			watchID = fileSystemWatcher.addWatch(Utils::GetAssetFolder(), &updateListener, true);
			fileSystemWatcher.watch();

			/* Send a Project Loaded Event */
			ServiceManager::Get<EventService>(ServiceManager::Events)->Publish(Events::EventType::ProjectLoaded,
				std::make_unique<Events::ProjectLoadedArgs>(_LoadedProject->initialScene, _LoadedProject, Utils::GetPath(projectfile)));


			if (Utils::FileExists(_LoadedProject->initialScene) == true)
			{
				LoadScene(_LoadedProject->initialScene);
			}
			else
			{
				WYRD_WARN("Unable to find scene {0}. Creating new Scene...", _LoadedProject->initialScene);
				CreateNewScene("Unknown");
			}

			result = true;
		}
		else
		{
			_LoadedProject = nullptr;
			result = false;
		}

		return result;
	}

	bool WorkspaceService::CreateNewScene(const std::string& name)
	{
		/* Create a new scene shared pointer */
		_LoadedScene = std::make_shared<Scene>(name);

		/* Mark scene loaded */
		IsSceneLoaded(true);

		/* fire scene loaded event on the editor system */
		ServiceManager::Get<EventService>(ServiceManager::Events)->Publish(Editor::Events::EventType::SceneOpened, std::make_unique<Events::SceneOpenedArgs>(_LoadedScene));

		return true;
	}

	bool WorkspaceService::LoadScene(const std::string& path)
	{
		_LoadedScene = std::make_shared<Scene>();
		_LoadedScene->Initialise();

		/* Load the scene object */
		SceneLoader::Result result = SceneLoader::Load(path, *_LoadedScene);

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
			ServiceManager::Get<EventService>(ServiceManager::Events)->Publish(Editor::Events::EventType::SceneOpened, std::make_unique<Events::SceneOpenedArgs>(_LoadedScene));

			return true;
		}
	}

	bool WorkspaceService::ReloadScene()
	{
		return LoadScene(_LoadedScenePath);
	}

	bool WorkspaceService::CloseScene()
	{
		/* Send a Scene Close Event */
		ServiceManager::Get<EventService>(ServiceManager::Events)->Publish(Events::EventType::SceneClosed, std::make_unique<Events::SceneClosedArgs>());

		_LoadedScene = nullptr;

		return true;
	}

	bool WorkspaceService::SaveScene()
	{
		std::string tempSaveFile = _LoadedScenePath;
		SceneLoader::Result result = SceneLoader::Save(tempSaveFile, *_LoadedScene);

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
		SceneLoader::Result result = SceneLoader::Save(path, *_LoadedScene);

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
