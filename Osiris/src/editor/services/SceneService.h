#pragma once

#include "core/export.h"

#include "editor/services/IService.h"
#include "editor/datamodels/Scene.h"

namespace Osiris::Editor
{
	class SceneService : public IService
	{
	public:
		SceneService() {}
		~SceneService() {}

		// Inherited via IService
		virtual void OnCreate() override;
		virtual void OnDestroy() override;

		// Getters and Setters
		inline std::shared_ptr<Scene> GetLoadedScene() { return _loadedScene; }
		inline std::string& GetLoadedScenePath() { return _loadedScenePath; }
		inline bool IsSceneLoaded() { return _isSceneLoaded; }
		inline void IsSceneLoaded(bool value) { _isSceneLoaded = value; }

		bool CreateNewScene();
		bool LoadScene(const std::string& path);
		bool SaveScene();
		bool SaveScene(const std::string& path);
		
	private:
		bool _isSceneLoaded;
		std::string _loadedScenePath;
		std::shared_ptr<Scene> _loadedScene;

		
	};
}