#pragma once

/* core wyrd includes */
#include <core/export.h>
#include <core/scene/Scene.h>

/* local includes */
#include "support/FileWatcher.h"
#include "IService.h"
#include "datamodels/Project.h"

namespace Wyrd::Editor
{
	class Project;

	class WorkspaceService : public IService
	{
	public:
		WorkspaceService() {}
		~WorkspaceService() {}

		// Inherited via IService
		virtual void OnCreate() override;
		virtual void OnDestroy() override;

		void SetProjectRootDirectory(const std::filesystem::path& rootDir);

		inline std::shared_ptr<Project> GetCurrentProject() { return _LoadedProject; }
		inline std::string& GetLoadedProjectPath() { return _LoadedProjectPath; }
		inline bool IsProjectLoaded() { return _IsProjectLoaded; }
		inline void IsProjectLoaded(bool value) { _IsProjectLoaded = value; }

		inline std::shared_ptr<Scene> GetLoadedScene() { return _LoadedScene; }
		inline std::string& GetLoadedScenePath() { return _LoadedScenePath; }
		inline bool IsSceneLoaded() { return _IsSceneLoaded; }
		inline void IsSceneLoaded(bool value) { _IsSceneLoaded = value; }

		void CreateNewProject(std::string location, std::string sceneName, std::string name);
		bool SaveProject();
		bool LoadProject(std::string projectfile);
		bool ReloadScene();

		bool CreateNewScene(const std::string& name);
		bool LoadScene(const std::string& path);
		bool CloseScene();
		bool SaveScene();
		bool SaveSceneAs(const std::string& path);

		inline const std::filesystem::path& GetProjectRootDirectory() const { return _ProjectRootDirectory; }
		inline const std::filesystem::path& GetAssetsDirectory() const { return _AssetsDirectory; }
		inline const std::filesystem::path& GetBuildsDirectory() const { return _BuildDirectory; }
		inline const std::filesystem::path& GetCacheDirectory() const { return _CacheDirectory; }
		inline const std::filesystem::path& GetTempDirectory() const { return _TempDirectory; }

	private:
		bool _IsProjectLoaded = false;
		std::string _LoadedProjectPath;
		std::shared_ptr<Project> _LoadedProject;

		bool _IsSceneLoaded = false;
		std::string _LoadedScenePath;
		std::shared_ptr<Scene> _LoadedScene;

		std::filesystem::path _ProjectRootDirectory;
		std::filesystem::path _AssetsDirectory;
		std::filesystem::path _BuildDirectory;
		std::filesystem::path _CacheDirectory;
		std::filesystem::path _TempDirectory;

		FileWatcher _FileWatcher;
	};
}