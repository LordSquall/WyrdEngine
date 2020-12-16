#pragma once

/* core osiris includes */
#include <core/export.h>
#include <core/scene/Scene.h>

/* local includes */
#include "IService.h"
#include "datamodels/Project.h"

namespace Osiris::Editor
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

	private:
		bool _IsProjectLoaded = false;
		std::string _LoadedProjectPath;
		std::shared_ptr<Project> _LoadedProject;

		bool _IsSceneLoaded = false;
		std::string _LoadedScenePath;
		std::shared_ptr<Scene> _LoadedScene;
	};
}