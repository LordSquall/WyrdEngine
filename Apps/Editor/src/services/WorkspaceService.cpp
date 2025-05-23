/* core wyrd includes */
#include <wyrdpch.h>
#include <core/Log.h>

/* local includes */
#include "WorkspaceService.h"
#include "ServiceManager.h"
#include "loaders/SceneLoader.h"
#include "loaders/ProjectLoader.h"
#include "datamodels/EditorComponents.h"
#include "datamodels/RecentProjects.h"

/* external includes */
#include <jsonxx.h>

namespace Wyrd::Editor
{
	void Wyrd::Editor::WorkspaceService::OnCreate()
	{
		/* Initialise Defaults */
		IsProjectLoaded(false);

		/* Subscribe to Project lifecycle events */
		ServiceManager::Get<EventService>()->Subscribe(Events::EventType::CreateNewProject, [this](Events::EventArgs& args) {
			Events::CreateNewProjectArgs& a = (Events::CreateNewProjectArgs&)args;
			if (CreateNewProject(a.location, a.sceneName, a.name))
			{
				WYRD_INFO("Project Created!");
			}
			else
			{
				WYRD_ERROR("Unable to create Project!");
			}
		});

		ServiceManager::Get<EventService>()->Subscribe(Events::EventType::OpenProject, [this](Events::EventArgs& args) 
			{
				Events::OpenProjectArgs& a = (Events::OpenProjectArgs&)args;
				LoadProject(a.name);
			});

		ServiceManager::Get<EventService>()->Subscribe(Events::EventType::CloseProject, [this](Events::EventArgs& args) {});

		ServiceManager::Get<EventService>()->Subscribe(Events::EventType::SetSceneCamera, [this](Events::EventArgs& args) {
			Events::SetSceneCameraArgs& a = (Events::SetSceneCameraArgs&)args;
				_LoadedScene->SetPrimaryCameraEntity(a.entity);
			});
	}

	void Wyrd::Editor::WorkspaceService::OnDestroy()
	{
		EndFileWatcher();

		if (_LoadedProject != nullptr) SaveProject();
		if (_LoadedScene != nullptr) SaveScene();
	}


	void Wyrd::Editor::WorkspaceService::SetProjectRootDirectory(const std::filesystem::path& rootDir)
	{
		_ProjectRootDirectory = rootDir;
		_AssetsDirectory = rootDir / "Assets";
		_BuildDirectory = rootDir / ".builds";
		_CacheDirectory = rootDir / ".cache";
		_TempDirectory = rootDir / ".temp";
	}

	bool Wyrd::Editor::WorkspaceService::CreateNewProject(const std::filesystem::path& path, std::string sceneName, std::string name)
	{
		/* Create the folder for the project */
		if (!Utils::CreateFolder(path / name))
		{
			WYRD_ERROR("Unable to create folder at {0}", path / name);
			return false;
		}

		/* Build project file name */
		_LoadedProjectPath = path / name / (name + ".oproj");

		/* Create a new project shared pointer */
		_LoadedProject = std::make_shared<Project>(name);

		/* Clear any selection */
		ServiceManager::Get<EventService>()->Publish(Events::EventType::SelectedEntityChanged, std::make_unique<Events::SelectedEntityChangedArgs>(ENTITY_INVALID));

		/* Save the project object to disk */
		ProjectLoader::Result result = ProjectLoader::Save(_LoadedProjectPath, *_LoadedProject);

		if (result == ProjectLoader::Result::Success)
		{
			/* Mark project as loaded */
			IsProjectLoaded(true);

			/* Update the settings to ensure this project is loaded by default next time */
			ServiceManager::Get<SettingsService>()->Set(_LoadedProjectPath.string(), CONFIG_PROJECT, CONFIG_PROJECT__DEFAULT);
				
			/* Set the base root folder */
			SetProjectRootDirectory(_LoadedProjectPath.parent_path());
			
			/* Create default folders and files */
			if (!Utils::CreateFolder(_AssetsDirectory))
			{
				WYRD_ERROR("Unable to create folder at {0}", _AssetsDirectory);
				return false;
			}

			if (!Utils::CreateFolder(_BuildDirectory))
			{
				WYRD_ERROR("Unable to create folder at {0}", _BuildDirectory); 
				return false;
			}

			if (!Utils::CreateFolder(_CacheDirectory))
			{
				WYRD_ERROR("Unable to create folder at {0}", _CacheDirectory);
				return false;
			}

			if (!Utils::CreateFolder(_TempDirectory))
			{
				WYRD_ERROR("Unable to create folder at {0}", _TempDirectory);
				return false;
			}

			/* Create a new default scene */
			CreateNewScene(sceneName);

			/* Set the default scene path */
			_LoadedScenePath = GetAssetsDirectory().string() + "\\" + sceneName + ".scene";

			/* Save the new scene into a known location */
			SaveScene();

			/* Set the default scene within the project model */
			_LoadedProject->initialScene = GetAssetsDirectory().string() + "\\" + sceneName + ".scene";

			/* create VS workspace file model */
			jsonxx::Object root;
			jsonxx::Array folders;
			jsonxx::Object assetsFolder;
			assetsFolder << "path" << "assets";
			folders << assetsFolder;

			root << "folders" << folders;
			std::ofstream out(_LoadedProjectPath.parent_path() / "/vs.code-workspace");
			out << root.json();
			out.close();

			StartFileWatcher();

			/* Send a Project Loaded Event */
			ServiceManager::Get<EventService>()->Publish(Events::EventType::ProjectLoaded,
				std::make_unique<Events::ProjectLoadedArgs>(_LoadedProject->initialScene, _LoadedProject, _LoadedProjectPath.parent_path()));
		}
	}

	bool Wyrd::Editor::WorkspaceService::SaveProject()
	{
		ProjectLoader::Result result = ProjectLoader::Result::Success;
		
		/* If a scene is loaded we want it to be set as the initial scene in the project model to ensure it gets loaded on restart */
		if (_LoadedScene != nullptr) _LoadedProject->initialScene = _LoadedScenePath.string();
		
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

	bool Wyrd::Editor::WorkspaceService::LoadProject(const std::filesystem::path& projectfile)
	{
		bool result = false;

		/* Store the loaded project path */
		_LoadedProjectPath = projectfile.string();

		/* Create a new project shared pointer */
		_LoadedProject = std::make_shared<Project>();

		/* Load the project object */
		ProjectLoader::Result r = ProjectLoader::Load(projectfile, *_LoadedProject);

		if (r == ProjectLoader::Result::Success)
		{
			/* Mark project as loaded */
			IsProjectLoaded(true);

			/* Add to recents list */
			AddToRecentProjects(_LoadedProject->name, _LoadedProjectPath);

			/* Update the settings to ensure this project is loaded by default next time */
			ServiceManager::Get<SettingsService>()->Set(_LoadedProjectPath.string(), CONFIG_PROJECT, CONFIG_PROJECT__DEFAULT);

			/* Set the project root directory path */
			SetProjectRootDirectory(projectfile.parent_path().string());

			/* Send a Project Loaded Event */
			ServiceManager::Get<EventService>()->Publish(Events::EventType::ProjectLoaded,
				std::make_unique<Events::ProjectLoadedArgs>(_LoadedProject->initialScene, _LoadedProject, projectfile.parent_path().string()));

			if (Utils::FileExists(_LoadedProject->initialScene) == true)
			{
				LoadScene(_LoadedProject->initialScene);
			}
			else
			{
				WYRD_WARN("Unable to find scene {0}. Creating new Scene...", _LoadedProject->initialScene);
				CreateNewScene("Unknown");
			}

			StartFileWatcher();

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
		_LoadedScene->Initialise();
		_LoadedScene->RegisterComponent<EditorComponent>("Editor", "EditorComponent", false, &EditorComponent::ResetFunc);

		/* Setup initial scene settings */
		_LoadedScene->cameraZoom = 500.0f;

		/* Mark scene loaded */
		IsSceneLoaded(true);

		/* fire scene loaded event on the editor system */
		ServiceManager::Get<EventService>()->Publish(Editor::Events::EventType::SceneOpened, std::make_unique<Events::SceneOpenedArgs>(_LoadedScene));

		return true;
	}

	bool WorkspaceService::LoadScene(const std::filesystem::path& path)
	{
		_LoadedScene = std::make_shared<Scene>();
		_LoadedScene->Initialise();
		_LoadedScene->RegisterComponent<EditorComponent>("Editor", "EditorComponent", false, &EditorComponent::ResetFunc);

		/* Load the scene object */
		SceneLoader::Result result = SceneLoader::Load(path, *_LoadedScene, true);

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
			ServiceManager::Get<EventService>()->Publish(Editor::Events::EventType::SceneOpened, std::make_unique<Events::SceneOpenedArgs>(_LoadedScene));

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
		ServiceManager::Get<EventService>()->Publish(Events::EventType::SceneClosed, std::make_unique<Events::SceneClosedArgs>());

		_LoadedScene = nullptr;

		return true;
	}

	bool WorkspaceService::SaveScene()
	{
		std::filesystem::path tempSaveFile = _LoadedScenePath;
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
			_LoadedProject->initialScene = _LoadedScenePath.string();
			SaveProject();

			return true;
		}
	}

	bool WorkspaceService::SaveSceneAs(const std::filesystem::path& path)
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
			_LoadedProject->initialScene = _LoadedScenePath.string();
			SaveProject();

			return true;
		}
	}

	void WorkspaceService::StartFileWatcher()
	{
		if (_FileWatcher.Running)
			EndFileWatcher();

		_FileWatcher.Initialise(GetAssetsDirectory().string(), std::chrono::milliseconds(500));
		_FileWatcher.Start([](std::string path, FileStatus status) -> void {
			if (!std::filesystem::is_regular_file(std::filesystem::path(path)) && status != FileStatus::erased)
			{
				return;
			}

			/* create filename name */
			auto lastSlash = path.find_last_of("/\\");
			lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
			std::string name = path.substr(lastSlash, path.size() - lastSlash);

			bool isDir = std::filesystem::is_directory(path);

			switch (status)
			{
			case FileStatus::created:
				std::cout << "File Created: " << path << std::endl;
				ServiceManager::Get<EventService>()->Publish(Events::EventType::AddFileEntry, std::make_unique<Events::AddFileEntryArgs>(path, path, isDir));
				break;

			case FileStatus::modified:
				std::cout << "File Modified: " << path << std::endl;
				ServiceManager::Get<EventService>()->Publish(Events::EventType::ModifiedFileEntry, std::make_unique<Events::ModifiedFileEntryArgs>(path, path, isDir));
				break;

			case FileStatus::erased:
				std::cout << "File Erase: " << path << std::endl;
				break;
			}
			});

	}

	void WorkspaceService::EndFileWatcher()
	{
		_FileWatcher.End();
	}

	void WorkspaceService::AddToRecentProjects(const std::string& name, const std::filesystem::path& path)
	{
		RecentProjects recentProjects;
		recentProjects.AddProject(name, path.string());
		recentProjects.Save();
	}
}
